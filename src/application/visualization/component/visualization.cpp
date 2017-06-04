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

    void VisualizationController::setOverlay(Overlay overlay) {

    }

    bool VisualizationController::getTerminateRequest() {
      return scene->isClosed();
    }

    void VisualizationController::update(
        float timeStep) {
      scene->update(timeStep, stream, viewMatrix);
    }

    void VisualizationController::setViewTransform(const glm::mat4x4 viewMatrix) {
      this->viewMatrix=viewMatrix;
    }

  }
}