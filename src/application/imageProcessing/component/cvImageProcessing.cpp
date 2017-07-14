//
// Created by moritz on 20.05.17.
//

#include "cvImageProcessing.h"

#include "OpenCVCameraStream.h"
#include "FileCameraStream.h"
#include "LazyCameraStream.h"

#include <opencv2/calib3d.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <algorithm>
#include <opencv2/imgproc.hpp>
#include <textureSynthesis/TextureSynthesis.h>

#include "smoothing/MeanSmoother.h"


namespace ape {
  namespace imageProcessing {

    CvImageProcessingController::CvImageProcessingController(
          glm::mat3x3 cameraIntrinsics,
          float* distCoeffs) :
        cvCameraStream(nullptr), lazyCameraStream(nullptr), cameraFrozen(false),
        searchedMarkerSignal(), marker(&searchedMarkerSignal),
        transformation(&searchedMarkerSignal),
        //FIXME magic numbers
        viewSmoother(MeanSmoother<cv::Vec3d>(5), MeanSmoother<cv::Vec3d>(3)),
        cameraIntrinsics(cameraIntrinsics), distCoeffs(distCoeffs),
        dictionary(cv::aruco::getPredefinedDictionary(
            cv::aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_250))),
        markerLength(0.024), //FIXME magic number ...
        ids(), corners(), rejected(),
        detectorParams(cv::aruco::DetectorParameters::create()),
        viewMatrix(), textureExtraction(), processingContext() {
      //these can throw ...
		    //cvCameraStream = new OpenCVCameraStream();
      auto stream= new FileCameraStream(
          "../../../data/dummy/cameraStream/board01.flv");
      stream->setSize(640,480);
      cvCameraStream=stream;
      lazyCameraStream = new LazyCameraStream(cvCameraStream);
      setProcessingContext(ProcessingContext::Context::Stream);
      detectorParams->doCornerRefinement = true; 
      detectorParams->cornerRefinementMaxIterations = 60;
      detectorParams->cornerRefinementMinAccuracy = 0.01;
    }

    CvImageProcessingController::~CvImageProcessingController() {
      delete lazyCameraStream;
      delete cvCameraStream;
    }

    // Source: http://stackoverflow.com/questions/3712049/how-to-use-an-opencv-rotation-and-translation-vector-with-opengl-es-in-android
    //FIXME move to helper class
    static glm::mat4x4 convertVectorsToViewMatrix(cv::Vec3d rotation, cv::Vec3d translation) {
      // rotation matrix
      cv::Mat rotMat = cv::Mat::zeros(3, 3, CV_64F);
      glm::mat4x4 viewMatrix(0.0);

      // rotation vectors can be converted to a 3-by-3 rotation matrix
	    cv::Rodrigues(rotation, rotMat);

    // std::cout << "Rotation Matrix: " << rotMat << std::endl;

      //Complete matrix ready to use
      for (unsigned int row = 0; row<3; ++row)
      {
        for (unsigned int col = 0; col<3; ++col)
        {
          viewMatrix[row][col] = (float)rotMat.at<double>(row, col);
        }
        viewMatrix[row][3] = (float)translation[row];
      }
      viewMatrix[3][3] = 1.0f;

      glm::mat4x4 cvToGl(0.0);
      cvToGl[0][0] = 1.0f;
      cvToGl[1][1] = -1.0f; // invert y axis
      cvToGl[2][2] = -1.0f; // invert z axis
      cvToGl[3][3] = 1.0f;
      viewMatrix = cvToGl * viewMatrix;

      //FIX ME Manuel fix by comparison with correct view matrix
      viewMatrix[0][1] = -viewMatrix[0][1];
      viewMatrix[0][2] = -viewMatrix[0][2];
      viewMatrix[1][0] = -viewMatrix[1][0];
      viewMatrix[2][0] = -viewMatrix[2][0];

      viewMatrix[0][3] = (float)translation[0];
      viewMatrix[1][3] = -(float)translation[1];
      viewMatrix[2][3] = -(float)translation[2];

      //FIXME only print in debug mode
      glm::vec4 test;
     // std::cout<<glm::to_string(viewMatrix)<<std::endl;

      return viewMatrix;
    }

    void CvImageProcessingController::updateTransformation() {
      searchedMarkerSignal.set();

      auto camStream = getCameraStream();
      auto frame=camStream->getCurrentFrame();

      // detect markers and estimate pose
      cv::aruco::detectMarkers(
          frame, dictionary, corners, ids, detectorParams, rejected);

      cv::Mat cameraIntrinsics_(3,3,CV_64F);

      //FIXME uhm......
      ((double*)cameraIntrinsics_.data)[0]=cameraIntrinsics[0][0];
      ((double*)cameraIntrinsics_.data)[1]=cameraIntrinsics[0][1];
      ((double*)cameraIntrinsics_.data)[2]=cameraIntrinsics[0][2];
      ((double*)cameraIntrinsics_.data)[3]=cameraIntrinsics[1][0];
      ((double*)cameraIntrinsics_.data)[4]=cameraIntrinsics[1][1];
      ((double*)cameraIntrinsics_.data)[5]=cameraIntrinsics[1][2];
      ((double*)cameraIntrinsics_.data)[6]=cameraIntrinsics[2][0];
      ((double*)cameraIntrinsics_.data)[7]=cameraIntrinsics[2][1];
      ((double*)cameraIntrinsics_.data)[8]=cameraIntrinsics[2][2];

      cv::Mat distCoeffs_(5, 1, CV_64F);
      ((double*)distCoeffs_.data)[0]=distCoeffs[0];
      ((double*)distCoeffs_.data)[1]=distCoeffs[1];
      ((double*)distCoeffs_.data)[2]=distCoeffs[2];
      ((double*)distCoeffs_.data)[3]=distCoeffs[3];
      ((double*)distCoeffs_.data)[4]=distCoeffs[4];



      cv::Ptr<cv::aruco::GridBoard> gridboard = cv::aruco::GridBoard::create(5, 5, float(0.012),
        float(0.0031), dictionary);
      cv::Ptr<cv::aruco::Board> board = gridboard.staticCast<cv::aruco::Board>();

      cv::aruco::refineDetectedMarkers(
        frame, board, corners, ids, rejected, cameraIntrinsics_, distCoeffs_);

      marker.setValue(ids.size() > 0);

      if (ids.size() > 3) {

        cv::Mat rvec, tvec;

        cv::aruco::estimatePoseBoard(
          corners, ids, board, cameraIntrinsics_, distCoeffs_, rvec, tvec);
#ifdef DEBUG_BUILD
        cv::aruco::drawDetectedMarkers(frame, corners, ids);
#endif
        viewSmoother.recordValue(ViewParameters<cv::Vec3d,cv::Vec3d>(
          tvec, rvec
        ));

        auto smoothedViewParams=viewSmoother.getSmoothedValue();

        viewMatrix = convertVectorsToViewMatrix(
          rvec, tvec);

#ifdef DEBUG_BUILD
        cv::aruco::drawAxis(
          frame, cameraIntrinsics_, distCoeffs_, rvec, tvec, markerLength);
#endif
        transformation.setValue(viewMatrix);
      }
      else {
        glm::mat4x4 viewMatrix(0.0);
        transformation.setValue(viewMatrix);
      }
    }

    void CvImageProcessingController::update(float timeDelta) {
      searchedMarkerSignal.reset();
      if (!cameraFrozen) {
        lazyCameraStream->pullFrame();
      }
    }

    CameraStream* CvImageProcessingController::getCameraStream() {
      return lazyCameraStream;
    }

    void CvImageProcessingController::freezeCameraStream(bool freeze) {
      this->cameraFrozen=freeze;
    }

    bool CvImageProcessingController::getTerminateRequest() {
      return false;

      //return !cap.isOpened()
    }

    bool CvImageProcessingController::hasMarker() {
      if (!searchedMarkerSignal) {
        updateTransformation();
      }

      return marker.get();
    }

    glm::mat4x4 CvImageProcessingController::getTransformation() {
      if (!searchedMarkerSignal) {
        updateTransformation();
      }

      return transformation.get();
    }

    void CvImageProcessingController::setProcessingContext(
        ProcessingContext::Context context) {
      switch (context) {
        case ProcessingContext::Context::Stream:
          processingContext.setStreamContext(getCameraStream());
          break;
        case ProcessingContext::Context::Image:
          //we currently do not freeze context
          //but simply freeze th whole camera stream
          //by doing so, we also get a frozen image in the display

          //TODO we 'freeze' the context to the current stream image
          //it may be better to allow arbitrary images to be set
          processingContext.setImageContext(getCameraStream()
                                                ->getCurrentFrame());
          break;
      }
    }

    cv::Mat CvImageProcessingController::extractTextureFromStream(
        const cv::Rect regionOfInterest) {
      auto context=processingContext.getContextValue();
      return textureExtraction.extractRegionOfInterest(
          context, regionOfInterest);
    }

    cv::Mat CvImageProcessingController::createTile(int width, int height,
                                                    cv::Mat source) {
      cv::Mat result;
      TextureSynthesis textureSynthesis(
          source, width, height);
      //FIXME magic numbers, yay!
      textureSynthesis.generateTexture(5,result);
      return result;
    }

  }
}
