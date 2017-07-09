#pragma once

#include "../../../application/worldState/component/worldState.h"
#include <worldState/World.h>

namespace ape {
  namespace worldState {

    class ModelBasedWorld: public IWorld {
    private:
    protected:
      std::vector<IWorldEntity*> entities;
    public:
      //Default
      ModelBasedWorld();

      // Copy constructor
      ModelBasedWorld(const ModelBasedWorld&) = default;

      // Move constructor
      ModelBasedWorld(ModelBasedWorld&&) = default;

      // Copy assignment operator
      ModelBasedWorld& operator=(const ModelBasedWorld&)& = default;

      // Move assignment operator
      ModelBasedWorld& operator=(ModelBasedWorld&&)& = default;

      // Destructor
      virtual ~ModelBasedWorld() = default;


      virtual const std::vector<IWorldEntity*> getEntities() override;

      virtual IWorldEntity* getEntity(
          IWorldEntity::WorldEntityHandle entityHandle) override;

      void addEntity(IWorldEntity* entity);
    };

  }
}
