//
// Created by moritz on 28.05.17.
//

#include "LazyCameraStream.h"

namespace ape {
  namespace imageProcessing {

    LazyCameraStream::LazyCameraStream(CameraStream* stream):
        stream(stream), frame() {
      pullFrame();
    }

    void LazyCameraStream::pullFrame() {
      frame=stream->getCurrentFrame();
    }

    cv::Mat LazyCameraStream::getCurrentFrame() {
      return frame;
    }

    unsigned int LazyCameraStream::getFrameWidth() {
      return stream->getFrameWidth();
    }

    unsigned int LazyCameraStream::getFrameHeight() {
      return stream->getFrameHeight();
    }

  }
}