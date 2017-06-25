#pragma once
//
// Created by moritz on 20.05.17.
//

#include <imageProcessing/CameraStream.h>

#include "opencv2/videoio.hpp"

namespace ape {
  namespace imageProcessing {

    class OpenCVCameraStream: public CameraStream {
    private:
      unsigned int frameWidth;
      unsigned int frameHeight;
    protected:
      cv::VideoCapture cap;
      cv::Mat frame;
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

      virtual cv::Mat getCurrentFrame() override;

      virtual unsigned int getFrameWidth() override;

      virtual unsigned int getFrameHeight() override;
    };

  }
}


