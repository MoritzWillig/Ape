#pragma once

#include "CameraStream.h"
#include "opencv2/core/mat.hpp"

namespace ape {
  namespace imageProcessing {

    class ProcessingContext {
    private:
    protected:
    public:
      enum class Context {
        Stream,
        Image,
      };

      virtual Context getContext() = 0;

      virtual void setStreamContext(CameraStream* stream) = 0;

      virtual void setImageContext(cv::Mat mat) = 0;
    };

  }
}