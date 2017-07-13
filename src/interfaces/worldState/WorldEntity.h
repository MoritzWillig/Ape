#pragma once

#include "./Surface.h"

namespace ape {
  namespace worldState {

    class IWorldEntity {
    private:
    protected:
    public:
      typedef int WorldEntityHandle;

      virtual ISurface::SurfacePersistentHandle getSurface() = 0;

      virtual void setSurface(ISurface::SurfacePersistentHandle surface) = 0;

      virtual WorldEntityHandle getHandle() = 0;
    };

  }
}
