#include "ModelBasedWorld.h"

namespace ape {
  namespace worldState {

    ModelBasedWorld::ModelBasedWorld() :
        entities() {
    }

    const std::vector<IWorldEntity*> ModelBasedWorld::getEntities() {
      return entities;
    }

    IWorldEntity* ModelBasedWorld::getEntity(
        IWorldEntity::WorldEntityHandle entityHandle) {
      auto it=std::find_if(entities.begin(),entities.end(),
                           [entityHandle](IWorldEntity* entity) -> bool {
                             return (entity->getHandle()==entityHandle);
                           });
      if (it==entities.end()) {
        throw std::runtime_error("unknown entity handle");
      }

      return *it;
    }

    void ModelBasedWorld::addEntity(IWorldEntity* entity) {
      entities.push_back(entity);
    }

  }
}
