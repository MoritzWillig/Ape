//
// Created by moritz on 20.05.17.
//

#include "imageProcessing.h"

#include "OpenCVCameraStream.h"

namespace ape {
  namespace imageProcessing {

    ImageProcessingController::ImageProcessingController():
        cameraStream(nullptr) {
      cameraStream=new OpenCVCameraStream();
    }

    void ImageProcessingController::update(float timeDelta) {

    }

    CameraStream* ImageProcessingController::getCameraStream() {
      return cameraStream;
    }

    bool ImageProcessingController::getTerminateRequest() {
      return false;

      // !cap.isOpened()
    }

  }
}
