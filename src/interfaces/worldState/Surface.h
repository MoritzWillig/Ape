#pragma once

#include <string>
#include <opencv2/core/mat.hpp>

#include <common/signals/Signal.h>

namespace ape {
  namespace worldState {

    class ISurface {
    private:
    protected:
    public:
      typedef int SurfacePersistentHandle;

      virtual std::string getName() = 0;

      virtual ValueSignal<SurfacePersistentHandle> getPersistentHandle() = 0;

      virtual cv::Mat getTexture() = 0;
    };

  }
}
