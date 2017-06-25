//
// Created by moritz on 20.05.17.
//

#include "imageProcessing.h"

#include "cvImageProcessing.h"

namespace ape {
  namespace imageProcessing {

    std::shared_ptr<IImageProcessingController>
    IImageProcessingController::createInstance(glm::mat3x3 cameraIntrinsics,
                                               float* distCoeffs) {
      return std::make_shared<CvImageProcessingController>(
          cameraIntrinsics, distCoeffs
      );
    }

  }
}
