//
// Created by moritz on 20.05.17.
//

#include "FileCameraStream.h"

#include <stdexcept>

#include <opencv2/videoio/videoio_c.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace ape {
  namespace imageProcessing {

    FileCameraStream::FileCameraStream(std::string path):
        frameWidth(0), frameHeight(0), cap(), frame(), loop(true) {
      //cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('A', 'V', 'C', '1'));
      cap.open(path);

      if (!cap.isOpened())
        throw std::runtime_error("Error opening a camera stream");

      cap.set(CV_CAP_PROP_CONVERT_RGB,1);

      frameWidth=(unsigned int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
      frameHeight=(unsigned int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    }

    cv::Mat FileCameraStream::getCurrentFrame() {
      cv::Mat src;//src image
      if (!cap.read(src)) {
        if (loop) {
          cap.set(CV_CAP_PROP_POS_AVI_RATIO, 0);
        }

        //try again
        if (!cap.read(src)) {
          throw std::runtime_error("Could not read frame from file stream.");
        }
      }

      return src;
    }

    unsigned int FileCameraStream::getFrameWidth() {
      return frameWidth;
    }

    unsigned int FileCameraStream::getFrameHeight() {
      return frameHeight;
    }

    void FileCameraStream::setSize(unsigned int width, unsigned int height) {
      cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
      cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
      frameWidth=width;
      frameHeight=height;
    }

    bool FileCameraStream::getLoop() {
      return loop;
    }

    void FileCameraStream::setLoop(bool loop) {
      this->loop=loop;
    }

  }
}
