#pragma once

#include <memory>
#include <opencv2/core/mat.hpp>

namespace ape {
  namespace imageProcessing {

    class CameraStream {
    private:
    protected:
    public:
      virtual cv::Mat getCurrentFrame() = 0;

      virtual unsigned int getFrameWidth() = 0;

      virtual unsigned int getFrameHeight() = 0;
    };

  }
}
