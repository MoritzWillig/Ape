#include "worldState.h"

#include "worldStateImplementation.h"

namespace ape {
  namespace worldState {

    std::shared_ptr<IWorldStateController> IWorldStateController::createInstance() {
      return std::make_shared<WorldStateImplementation>();
    }

  }
}