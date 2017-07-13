#pragma once

#include <worldState/WorldEntity.h>
#include <visualization/VisualModel.h>
#include <common/callbacks/CustomValueCallback.h>

namespace ape {
  namespace worldState {

    class ModelBasedWorldEntity : public IWorldEntity {
    private:
    protected:
      ape::visualization::IVisualModel::VisualModelHandle visualHandle;
      WorldEntityHandle entityHandle;

      ISurface::SurfacePersistentHandle surface;
    public:
      //Default
      ModelBasedWorldEntity() = delete;

      ModelBasedWorldEntity(
          ape::visualization::IVisualModel::VisualModelHandle visualHandle,
          WorldEntityHandle entityHandle
      );

      // Copy constructor
      ModelBasedWorldEntity(const ModelBasedWorldEntity&) = default;

      // Move constructor
      ModelBasedWorldEntity(ModelBasedWorldEntity&&) = default;

      // Copy assignment operator
      ModelBasedWorldEntity& operator=(const ModelBasedWorldEntity&)& = default;

      // Move assignment operator
      ModelBasedWorldEntity& operator=(ModelBasedWorldEntity&&)& = default;

      // Destructor
      virtual ~ModelBasedWorldEntity() = default;


      virtual ISurface::SurfacePersistentHandle getSurface() override;

      virtual void setSurface(ISurface::SurfacePersistentHandle surface) override;

      virtual WorldEntityHandle getHandle() override;

      ape::visualization::IVisualModel::VisualModelHandle getVisualHandle();

      typedef void (*SurfaceChangedHandler)(
          void* custom,
          ModelBasedWorldEntity* entity);

      CustomValueCallback<SurfaceChangedHandler,void*> surfaceChangedHandler;
    };

  }
}