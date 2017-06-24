#pragma once

#include "imageProcessing/CameraStream.h"
#include "imageProcessing/ProcessingContext.h"

#include <memory>
#include <opencv2/core/mat.hpp>

#include "glm/glm.hpp"

namespace ape {
  namespace imageProcessing {

    class IImageProcessingController {
    private:
    protected:
    public:
      //Default
      IImageProcessingController() = default;

      // Copy constructor
      IImageProcessingController(const IImageProcessingController&) = default;

      // Move constructor
      IImageProcessingController(IImageProcessingController&&) = default;

      // Copy assignment operator
      IImageProcessingController&
      operator=(const IImageProcessingController&)& = default;

      // Move assignment operator
      IImageProcessingController&
      operator=(IImageProcessingController&&)& = default;

      // Destructor
      virtual ~IImageProcessingController() = default;

      virtual void update(float timeDelta)=0;

      virtual CameraStream* getCameraStream() = 0;

      virtual bool getTerminateRequest() = 0;

      //AR tracking
      virtual bool hasMarker() = 0;

      virtual glm::mat4x4 getTransformation() = 0;

      //texture synthesis
      virtual cv::Mat extractTextureFromStream(const cv::Rect regionOfInterest)
        = 0;

      virtual void setProcessingContext(ProcessingContext::Context context) = 0;

      static std::shared_ptr<IImageProcessingController> createInstance(
          glm::mat3x3 cameraIntrinsics,
          float* distCoeffs
      );

    };

  }
}

