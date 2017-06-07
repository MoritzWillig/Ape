#pragma once

#include <memory>

#include <imageProcessing/CameraStream.h>
#include "glm/glm.hpp"

namespace ape {
  namespace visualization {

    class IVisualizationController {
    private:
    protected:
    public:
      enum class Overlay {
        Loading,
        Menu,
        WorldScreen
      };

      virtual void setOverlay(Overlay overlay) = 0;

      virtual void update(float timeStep) = 0;

      virtual bool getTerminateRequest() = 0;

      virtual void setViewTransform(const glm::mat4x4 viewMatrix) = 0;

      virtual void setProgress(float d) = 0;


      static std::shared_ptr<IVisualizationController> createInstance(
          imageProcessing::CameraStream* stream
      );
    };
  }
}