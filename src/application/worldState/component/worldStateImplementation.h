#pragma once

#include "worldState.h"

namespace ape {
  namespace worldState {

    class WorldStateImplementation : public IWorldStateController {
    private:
    protected:
    public:
      //Default
      WorldStateImplementation() = default;

      // Copy constructor
      WorldStateImplementation(const WorldStateImplementation&) = default;

      // Move constructor
      WorldStateImplementation(WorldStateImplementation&&) = default;

      // Copy assignment operator
      WorldStateImplementation&
      operator=(const WorldStateImplementation&)& = default;

      // Move assignment operator
      WorldStateImplementation&
      operator=(WorldStateImplementation&&)& = default;

      // Destructor
      virtual ~WorldStateImplementation() = default;

      virtual void update(float timeStep) override;
    };

  }
}

