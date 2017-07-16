//
// Created by moritz on 20.05.17.
//

#include "OpenCVCameraStream.h"

#include <stdexcept>
#include <iostream>


#include <opencv2/videoio/videoio_c.h>


namespace ape {
  namespace imageProcessing {

    OpenCVCameraStream::OpenCVCameraStream():
        frameWidth(0), frameHeight(0), cap(CV_CAP_DSHOW | 0), frame() { // -1 gets any camera
      if (!cap.isOpened())
        throw std::runtime_error("Error opening a camera stream");

      std::cout << "Camera Bild" << std::endl;
      cap.set(CV_CAP_PROP_CONVERT_RGB,1);
      std::cout << "Camera Bild" << std::endl;

      frameWidth=(unsigned int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
      frameHeight=(unsigned int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    }

    cv::Mat OpenCVCameraStream::getCurrentFrame() {
      if (!cap.read(frame)) {
        throw std::runtime_error("Could not read frame from camera.");
      }

      return frame;
    }

    unsigned int OpenCVCameraStream::getFrameWidth() {
      return frameWidth;
    }

    unsigned int OpenCVCameraStream::getFrameHeight() {
      return frameHeight;
    }

  }
}
