#include "visualization.h"

#include "visualizationOGRE.h"

namespace ape {
  namespace visualization {

    IVisualizationController::IVisualizationController():
        overlayChangeRequestHandler(nullptr,nullptr) {
    }

    std::shared_ptr<IVisualizationController> IVisualizationController::createInstance(
        imageProcessing::CameraStream* stream
    ) {
      return std::make_shared<OGREVisualizationController>(stream);
    }

  }
}