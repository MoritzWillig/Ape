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
      //Default
      CameraStream() = default;

      // Copy constructor
      CameraStream(const CameraStream&) = default;

      // Move constructor
      CameraStream(CameraStream&&) = default;

      // Copy assignment operator
      CameraStream& operator=(const CameraStream&)& = default;

      // Move assignment operator
      CameraStream& operator=(CameraStream&&)& = default;

      // Destructor
      virtual ~CameraStream() = default;

      virtual void* getCurrentFrame() = 0;
    };

  }
}
