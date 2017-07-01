#pragma once

#include <memory>
#include <vector>
#include <string>
#include <opencv2/core/mat.hpp>

#include <common/signals/Signal.h>

namespace ape {
  namespace worldState {

    class ISurface {
    private:
    protected:
    public:
      typedef int SurfacePersistentHandle;

      virtual std::string getName() = 0;
      virtual ValueSignal<SurfacePersistentHandle> getPersistentHandle() = 0;
      virtual cv::Mat getTexture() = 0;
    };

    class IWorldStateController {
    private:
    protected:
    public:
      virtual ISurface::SurfacePersistentHandle createSurface(
          std::string name, cv::Mat texture) = 0;

      virtual bool removeSurface(
          ISurface::SurfacePersistentHandle surfaceHandle) = 0;

      virtual void persistSurface(
          ISurface::SurfacePersistentHandle surfaceHandle) = 0;

      virtual std::shared_ptr<ISurface> getSurface(
          ISurface::SurfacePersistentHandle surfaceHandle) = 0;

      virtual std::vector<
          ISurface::SurfacePersistentHandle> getSurfaceHandles() = 0;

      virtual void update(float timeStep) = 0;

      static std::shared_ptr<IWorldStateController> createInstance(
          std::string surfaceDatabasePath,
          std::string surfaceDatabaseName
      );
    };
  }
}