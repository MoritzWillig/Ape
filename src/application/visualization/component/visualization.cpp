#include "visualization.h"


namespace ape {
  namespace visualization {

    VisualizationController::VisualizationController():
      scene(nullptr) {
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

    void VisualizationController::update(float timeStep) {
      scene->update(timeStep);
    }

  }
}