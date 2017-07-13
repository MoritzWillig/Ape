#pragma once

#include "./WorldEntity.h"

namespace ape {
  namespace worldState {

    class IWorld {
    private:
    protected:
    public:
      virtual const std::vector<IWorldEntity*> getEntities() = 0;

      virtual IWorldEntity* getEntity(
          IWorldEntity::WorldEntityHandle entityHandle) = 0;
    };

  }
}
