#pragma once

#include <memory>

#include <imageProcessing/CameraStream.h>
#include "glm/glm.hpp"
#include <common/callbacks/CustomValueCallback.h>

namespace ape {
  namespace visualization {

    class IVisualizationController {
    private:
    protected:
    public:
      IVisualizationController();

      enum class Overlay {
        Loading,
        Menu,
        WorldScreen,
        TextureSynthesisSelection
      };

      virtual void setOverlay(Overlay overlay, bool enable) = 0;

      virtual void update(float timeStep) = 0;

      virtual bool getTerminateRequest() = 0;

      virtual void setViewTransform(const glm::mat4x4 viewMatrix) = 0;

      virtual void setProgress(float d) = 0;

      typedef void (*OverlayChangeRequestHandler)(void* custom, Overlay overlay);

      CustomValueCallback<
          IVisualizationController::OverlayChangeRequestHandler,
          void*> overlayChangeRequestHandler;

      static std::shared_ptr<IVisualizationController> createInstance(
          imageProcessing::CameraStream* stream
      );
    };
  }
}