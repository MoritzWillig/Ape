#pragma once
//
// Created by moritz on 20.05.17.
//

#include "imageProcessing/CameraStream.h"

namespace ape {
  namespace imageProcessing {

    class ImageProcessingController {
    private:
 //     CameraStream* cameraStream;
    protected:
    public:
      //Default
      ImageProcessingController();

      // Copy constructor
      ImageProcessingController(const ImageProcessingController&) = default;

      // Move constructor
      ImageProcessingController(ImageProcessingController&&) = default;

      // Copy assignment operator
      ImageProcessingController&
      operator=(const ImageProcessingController&)& = default;

      // Move assignment operator
      ImageProcessingController&
      operator=(ImageProcessingController&&)& = default;

      // Destructor
      virtual ~ImageProcessingController() = default;

      void update(float timeDelta);

 //     CameraStream* getCameraStream();

      bool getTerminateRequest();

    };

  }
}

