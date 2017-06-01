#include "visualization.h"

#include "AppWindow.h"

namespace ape {
  namespace visualization {

    VisualizationController::VisualizationController(
        imageProcessing::CameraStream* stream):
      stream(stream), scene(nullptr) {
    }

    void VisualizationController::startDisplay() {
      if (scene != nullptr) {
        throw std::runtime_error("Display already initialized");
      }

      scene=new AppWindow();
    }

    void VisualizationController::showMenus() {

    }

    void VisualizationController::showScene() {

    }

    bool VisualizationController::getTerminateRequest() {
      return scene->isClosed();
    }

    void VisualizationController::update(
        float timeStep, const glm::mat4& viewMatrix) {
      scene->update(timeStep, stream, viewMatrix);
    }

  }
}