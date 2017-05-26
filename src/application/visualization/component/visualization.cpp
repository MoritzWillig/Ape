#include "visualization.h"

#include "AppWindow.h"

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

    void VisualizationController::update(float timeStep, unsigned char* frameData, int width, int height, double* viewMatrix) {
      scene->update(timeStep, frameData, width, height, viewMatrix);
    }

  }
}