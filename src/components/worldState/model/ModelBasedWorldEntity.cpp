#include "ModelBasedWorldEntity.h"

namespace ape {
  namespace worldState {

    ModelBasedWorldEntity::ModelBasedWorldEntity(
        ape::visualization::IVisualModel::VisualModelHandle visualHandle,
        WorldEntityHandle entityHandle):
        visualHandle(visualHandle), entityHandle(entityHandle) {
    }

    ISurface::SurfacePersistentHandle ModelBasedWorldEntity::getSurface() {
      //FIXME implement
      throw std::runtime_error("Not implemented");
    }

    void ModelBasedWorldEntity::setSurface(
        ISurface::SurfacePersistentHandle surface) {
      //FIXME implement
      throw std::runtime_error("Not implemented");
    }

    IWorldEntity::WorldEntityHandle ModelBasedWorldEntity::getHandle() {
      return entityHandle;
    }

  }
}