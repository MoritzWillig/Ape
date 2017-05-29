//
// Created by moritz on 28.05.17.
//

#include "LazyCameraStream.h"

namespace ape {
  namespace imageProcessing {

    LazyCameraStream::LazyCameraStream(CameraStream* stream):
        stream(stream), frame(nullptr) {
      pullFrame();
    }

    void LazyCameraStream::pullFrame() {
      frame=stream->getCurrentFrame();
    }

    void* LazyCameraStream::getCurrentFrame() {
      return frame;
    }

    unsigned int LazyCameraStream::getFrameWidth() {
      return stream->getFrameWidth();
    }

    unsigned int LazyCameraStream::getFrameHeight() {
      return stream->getFrameWidth();
    }

  }
}