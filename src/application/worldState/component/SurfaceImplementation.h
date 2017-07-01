#pragma once

#include <string>
#include <common/signals/Signal.h>

#include "worldState.h"


namespace ape {
  namespace worldState {

    class SurfaceImplementation: public ISurface {
    private:
      ValueSignal<std::string> path;

      ValueSignal<bool> isExisting;
    protected:
      ValueSignal<ISurface::SurfacePersistentHandle> handle;
      std::string name;

      void probeSurfaceExisting();
    public:
      //Default
      SurfaceImplementation();

      SurfaceImplementation(
          std::string path, ISurface::SurfacePersistentHandle handle,
          std::string name);

      // Copy constructor
      SurfaceImplementation(const SurfaceImplementation&) = default;

      // Move constructor
      SurfaceImplementation(SurfaceImplementation&&) = default;

      // Copy assignment operator
      SurfaceImplementation& operator=(const SurfaceImplementation&)& = default;

      // Move assignment operator
      SurfaceImplementation& operator=(SurfaceImplementation&&)& = default;

      // Destructor
      virtual ~SurfaceImplementation() = default;


      virtual std::string getName() override;

      virtual ValueSignal<ISurface::SurfacePersistentHandle>
        getPersistentHandle() override;
    };

  }
}
