#pragma once
//
// Created by moritz on 20.05.17.
//

namespace ape {
  namespace imageProcessing {

    class CameraStream {
    private:
    protected:
    public:
      virtual void* getCurrentFrame() = 0;

      virtual unsigned int getFrameWidth() = 0;

      virtual unsigned int getFrameHeight() = 0;
    };

  }
}
