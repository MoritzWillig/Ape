#include "visualization.h"


namespace ape {
  namespace visualization {

    VisualizationController::VisualizationController() {

    }

    AppWindow* VisualizationController::getScene() {
      return new AppWindow();
    }
  }
}