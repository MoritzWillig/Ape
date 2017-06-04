#pragma once

#include <imageProcessing/CameraStream.h>
#include "glm/glm.hpp"

namespace ape {
  namespace visualization {

    class AppWindow;

    class VisualizationController {
    private:
      imageProcessing::CameraStream* stream;
      AppWindow* scene;

      glm::mat4x4 viewMatrix;
    protected:
    public:
      VisualizationController(imageProcessing::CameraStream* pStream);

      VisualizationController() = delete;

      void startDisplay();

      enum class Overlay {
        Loading,
        Menu,
        WorldScreen
      };

      void setOverlay(Overlay overlay);

	    void update(float timeStep);

      bool getTerminateRequest();

      void setViewTransform(const glm::mat4x4 viewMatrix);
    };
  }
}