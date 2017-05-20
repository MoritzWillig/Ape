#pragma once
//
// Created by moritz on 20.05.17.
//

#include "imageProcessing/CameraStream.h"

namespace ape {
  namespace imageProcessing {

    class OpenCVCameraStream: public CameraStream {
    private:
    protected:
    public:
      //Default
      OpenCVCameraStream();

      // Copy constructor
      OpenCVCameraStream(const OpenCVCameraStream&) = default;

      // Move constructor
      OpenCVCameraStream(OpenCVCameraStream&&) = default;

      // Copy assignment operator
      OpenCVCameraStream& operator=(const OpenCVCameraStream&)& = default;

      // Move assignment operator
      OpenCVCameraStream& operator=(OpenCVCameraStream&&)& = default;

      // Destructor
      virtual ~OpenCVCameraStream() = default;

      virtual void* getCurrentFrame() override;
    };

  }
}


