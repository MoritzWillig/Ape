#include "ModelBasedWorldEntity.h"

namespace ape {
  namespace worldState {

    ModelBasedWorldEntity::ModelBasedWorldEntity(
        ape::visualization::IVisualModel::VisualModelHandle visualHandle,
        WorldEntityHandle entityHandle):
        visualHandle(visualHandle), entityHandle(entityHandle),
        surface(-1), surfaceChangedHandler(nullptr, nullptr) {
    }

    ISurface::SurfacePersistentHandle ModelBasedWorldEntity::getSurface() {
      return surface;
    }

    void ModelBasedWorldEntity::setSurface(
        ISurface::SurfacePersistentHandle surface) {
      this->surface=surface;

      surfaceChangedHandler.callExceptIfNotSet(this);
    }

    IWorldEntity::WorldEntityHandle ModelBasedWorldEntity::getHandle() {
      return entityHandle;
    }

    ape::visualization::IVisualModel::VisualModelHandle
    ModelBasedWorldEntity::getVisualHandle() {
      return visualHandle;
    }

  }
}