#include "worldState.h"

#include "worldStateImplementation.h"

namespace ape {
  namespace worldState {

    std::shared_ptr<IWorldStateController> IWorldStateController::createInstance(
        std::string surfaceDatabasePath,
        std::string surfaceDatabaseName
    ) {
      return std::make_shared<WorldStateImplementation>(
          surfaceDatabasePath,
          surfaceDatabaseName);
    }

  }
}