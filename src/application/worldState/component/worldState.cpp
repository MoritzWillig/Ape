#include "worldState.h"

#include "worldFileBasedState.h"

namespace ape {
  namespace worldState {

    std::shared_ptr<IWorldStateController> IWorldStateController::createInstance(
        std::string surfaceDatabasePath,
        std::string surfaceDatabaseName,
        ape::visualization::IVisualizationController* visualizationController
    ) {
      return std::make_shared<WorldFileBasedStateController>(
          surfaceDatabasePath,
          surfaceDatabaseName,
          visualizationController);
    }

  }
}