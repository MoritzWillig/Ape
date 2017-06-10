//
// Created by moritz on 20.05.17.
//

#include "imageProcessing.h"

#include "OpenCVCameraStream.h"
#include "FileCameraStream.h"
#include "LazyCameraStream.h"

#include <opencv2/calib3d.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace ape {
  namespace imageProcessing {

    ImageProcessingController::ImageProcessingController(
          glm::mat3x3 cameraIntrinsics,
          float* distCoeffs) :
        cvCameraStream(nullptr), lazyCameraStream(nullptr),
        searchedMarkerSignal(), marker(&searchedMarkerSignal),
        transformation(&searchedMarkerSignal),
        cameraIntrinsics(cameraIntrinsics), distCoeffs(distCoeffs),
        dictionary(cv::aruco::getPredefinedDictionary(
            cv::aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_250))),
        markerLength(0.15), //FIXME magic number ...
        ids(), corners(), rejected(), rvecs(), tvecs(),
        detectorParams(cv::aruco::DetectorParameters::create()),
        viewMatrix() {
      //these can throw ...
      //cvCameraStream = new OpenCVCameraStream();
      auto stream= new FileCameraStream(
          "../../../data/dummy/cameraStream/marker01.avi");
      stream->setSize(640,480);
      cvCameraStream=stream;
      lazyCameraStream = new LazyCameraStream(cvCameraStream);
    }

    // Source: http://stackoverflow.com/questions/3712049/how-to-use-an-opencv-rotation-and-translation-vector-with-opengl-es-in-android
    //FIXME move to helper class
    static glm::mat4x4 convertVectorsToViewMatrix(cv::Vec3d rotation, cv::Vec3d translation) {
      // rotation matrix
      cv::Mat rotMat = cv::Mat::zeros(4, 4, CV_64F);
      glm::mat4x4 viewMatrix(0.0);

      // rotation vectors can be converted to a 3-by-3 rotation matrix
      cv::Rodrigues(rotation, rotMat);

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


      //FIXME only print in debug mode
      glm::vec4 test;
      std::cout<<glm::to_string(viewMatrix)<<std::endl;

      return viewMatrix;
    }

    void ImageProcessingController::updateTransformation() {
      searchedMarkerSignal.set();

      auto camStream = getCameraStream();
      cv::Mat frame(
          camStream->getFrameWidth(),
          camStream->getFrameHeight(),
          CV_8UC3, camStream->getCurrentFrame());

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

      cv::aruco::estimatePoseSingleMarkers(
          corners, markerLength, cameraIntrinsics_, distCoeffs_, rvecs, tvecs);

      marker.setValue(ids.size() > 0);

      if (marker) {
        cv::aruco::drawDetectedMarkers(frame, corners, ids);
        viewMatrix = convertVectorsToViewMatrix(rvecs[0], tvecs[0]);

        for (unsigned int i = 0; i < ids.size(); i++) {
          //FIXME add flag for and draw only in debug mode
          cv::aruco::drawAxis(frame, cameraIntrinsics_, distCoeffs_, rvecs[i], tvecs[i],
                              markerLength * 0.5f);
        }

        //TODO for now, we only care about the nearest marker
        transformation.setValue(viewMatrix);
      }
    }

    void ImageProcessingController::update(float timeDelta) {
      searchedMarkerSignal.reset();
      //FIXME cast because we dont want to show LazyCameraStream to the public
      ((LazyCameraStream*)lazyCameraStream)->pullFrame();
    }

    CameraStream* ImageProcessingController::getCameraStream() {
      return lazyCameraStream;
    }

    bool ImageProcessingController::getTerminateRequest() {
      return false;

      //return !cap.isOpened()
    }

    bool ImageProcessingController::hasMarker() {
      if (!searchedMarkerSignal) {
        updateTransformation();
      }

      return marker.get();
    }

    glm::mat4x4 ImageProcessingController::getTransformation() {
      if (!searchedMarkerSignal) {
        updateTransformation();
      }

      return transformation.get();
    }

  }
}
