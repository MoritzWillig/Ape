#include "visualization.h"

#include "visualizationOGRE.h"

namespace ape {
  namespace visualization {

    IVisualizationController::IVisualizationController():
        overlayChangeRequestHandler(nullptr,nullptr),
        surfaceSelectionHandler(nullptr,nullptr),
        entitySelectionHandler(nullptr,nullptr) {
    }

    std::shared_ptr<IVisualizationController> IVisualizationController::createInstance(
        imageProcessing::IImageProcessingController* ipController,
        imageProcessing::CameraStream* stream
    ) {
      return std::make_shared<OGREVisualizationController>(ipController, stream);
    }

  }
}