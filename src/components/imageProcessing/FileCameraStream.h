#pragma once
//
// Created by moritz on 20.05.17.
//

#include <imageProcessing/CameraStream.h>

#include "opencv2/videoio.hpp"

namespace ape {
  namespace imageProcessing {

    class FileCameraStream: public CameraStream {
    private:
      unsigned int frameWidth;
      unsigned int frameHeight;
    protected:
      cv::VideoCapture cap;
      cv::Mat frame;

      bool loop;
    public:
      //Default
      FileCameraStream() = delete;

      FileCameraStream(std::string file);

      // Copy constructor
      FileCameraStream(const FileCameraStream&) = default;

      // Move constructor
      FileCameraStream(FileCameraStream&&) = default;

      // Copy assignment operator
      FileCameraStream& operator=(const FileCameraStream&)& = default;

      // Move assignment operator
      FileCameraStream& operator=(FileCameraStream&&)& = default;

      // Destructor
      virtual ~FileCameraStream() = default;

      virtual void* getCurrentFrame() override;

      virtual unsigned int getFrameWidth() override;

      virtual unsigned int getFrameHeight() override;

      void setSize(unsigned int width, unsigned int height);

      bool getLoop();

      void setLoop(bool loop);
    };

  }
}


