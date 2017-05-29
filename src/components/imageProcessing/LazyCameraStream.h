#pragma once
//
// Created by moritz on 28.05.17.
//

#include <imageProcessing/CameraStream.h>

namespace ape {
  namespace imageProcessing {

    class LazyCameraStream : public CameraStream {
    private:
      CameraStream* stream;
      void* frame;
    protected:
    public:
      //Default
      LazyCameraStream() = delete;

      LazyCameraStream(CameraStream* stream);

      // Copy constructor
      LazyCameraStream(const LazyCameraStream&) = default;

      // Move constructor
      LazyCameraStream(LazyCameraStream&&) = default;

      // Copy assignment operator
      LazyCameraStream& operator=(const LazyCameraStream&)& = default;

      // Move assignment operator
      LazyCameraStream& operator=(LazyCameraStream&&)& = default;

      // Destructor
      virtual ~LazyCameraStream() = default;

      void pullFrame();

      virtual void* getCurrentFrame();
    };

  }
}
