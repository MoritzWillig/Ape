#pragma once

#include <memory>
#include <vector>
#include <string>
#include <opencv2/core/mat.hpp>

#include <common/signals/Signal.h>
#include <worldState/Surface.h>
#include <worldState/WorldEntity.h>
#include <worldState/World.h>
#include "../../visualization/component/visualization.h"

namespace ape {
  namespace worldState {

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

      virtual IWorld* loadWorld(std::string path) = 0;

      static std::shared_ptr<IWorldStateController> createInstance(
          std::string surfaceDatabasePath,
          std::string surfaceDatabaseName,
          ape::visualization::IVisualizationController* visualizationController
      );
    };
  }
}