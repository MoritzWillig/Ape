#include "visualization.h"

#include "AppWindow.h"
#include "visualizationOGRE.h"

namespace ape {
  namespace visualization {

    OGREVisualizationController::OGREVisualizationController(
        imageProcessing::CameraStream* stream):
        stream(stream), scene(nullptr) {
    }

    void OGREVisualizationController::startDisplay() {
      if (scene != nullptr) {
        throw std::runtime_error("Display already initialized");
      }

      scene=new AppWindow();
    }

    void OGREVisualizationController::setOverlay(Overlay overlay) {

    }

    bool OGREVisualizationController::getTerminateRequest() {
      return scene->isClosed();
    }

    void OGREVisualizationController::update(
        float timeStep) {
      scene->update(timeStep, stream, viewMatrix);
    }

    void OGREVisualizationController::setViewTransform(const glm::mat4x4 viewMatrix) {
      this->viewMatrix=viewMatrix;
    }

    void OGREVisualizationController::setProgress(float percent) {
      //TODO
    }

  }
}