#pragma once

#include "imageProcessing.h"

#include <list>

#include <glm/glm.hpp>
#include <opencv2/aruco/dictionary.hpp>
#include <opencv2/aruco.hpp>
#include <LazyCameraStream.h>

#include "imageProcessing/CameraStream.h"
#include "common/signals/Signal.h"

#include "textureSynthesis/TextureExtraction.h"
#include "ProcessingContextImplementation.h"
#include "smoothing/TransformationSmoother.h"

namespace ape {
  namespace imageProcessing {

    class CvImageProcessingController: public IImageProcessingController {
    private:
      //FIXME we dont want to show OpenCV- / LazyCameraStream to the public
      //but it would be nice to state the actual types here
      //(without using pointers)
      CameraStream* cvCameraStream;
      LazyCameraStream* lazyCameraStream;
      bool cameraFrozen;

      Signal searchedMarkerSignal;
      SignalOrDefault<bool> marker;
      SignalOrDefault<glm::mat4x4> transformation;

      TransformationSmoother<
          cv::Vec3d,cv::Vec3d,
          MeanSmoother<cv::Vec3d>,
          MeanSmoother<cv::Vec3d>> viewSmoother;

      void updateTransformation();

      glm::mat3x3 cameraIntrinsics;
      float* distCoeffs;

      //aruco
      cv::Ptr<cv::aruco::Dictionary> dictionary;
      float markerLength;
      std::vector<int> ids;
      std::vector<std::vector< cv::Point2f > > corners, rejected;
      std::vector<cv::Vec3d> rvecs, tvecs;
      cv::Ptr<cv::aruco::DetectorParameters> detectorParams;
      glm::mat4x4 viewMatrix;

      TextureExtraction textureExtraction;
      ProcessingContextImplementation processingContext;
    protected:
    public:
      //Default
      CvImageProcessingController() = delete;

      CvImageProcessingController(
          glm::mat3x3 cameraIntrinsics,
          float* distCoeffs);

      // Copy constructor
      CvImageProcessingController(const CvImageProcessingController&) = default;

      // Move constructor
      CvImageProcessingController(CvImageProcessingController&&) = default;

      // Copy assignment operator
      CvImageProcessingController&
      operator=(const CvImageProcessingController&)& = default;

      // Move assignment operator
      CvImageProcessingController&
      operator=(CvImageProcessingController&&)& = default;

      // Destructor
      virtual ~CvImageProcessingController();

      virtual void update(float timeDelta) override;

      virtual CameraStream* getCameraStream() override;

      virtual void freezeCameraStream(bool freeze) override;

      virtual bool getTerminateRequest() override;

      //AR tracking
      virtual bool hasMarker() override;

      virtual glm::mat4x4 getTransformation() override;

      virtual void setProcessingContext(ProcessingContext::Context context) override;

      //texture synthesis
      virtual cv::Mat extractTextureFromStream(
          const cv::Rect regionOfInterest)  override;

      virtual cv::Mat createTile(int width, int height, cv::Mat source) override;

    };

  }
}

