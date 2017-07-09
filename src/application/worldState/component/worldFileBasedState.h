#pragma once

#include <map>
#include <memory>

#include "worldState.h"
#include "SurfaceImplementation.h"
#include <worldState/World.h>

#include <model/ModelBasedWorld.h>
#include <model/ModelBasedWorldEntity.h>
#include <common/generators/Counter.h>

namespace ape {
  namespace worldState {

    class WorldFileBasedStateController : public IWorldStateController {
    private:
      std::string surfaceDatabasePath;
      std::string surfaceDatabaseName;

      Counter entityHandleGenerator;
    protected:
      ape::visualization::IVisualizationController* visController;

      void loadSurfaceDatabase();

      std::string getFileNameFromHandle(
          ISurface::SurfacePersistentHandle surfaceHandle);

      std::map<
          ISurface::SurfacePersistentHandle,
          std::shared_ptr<SurfaceImplementation>> surfaces;

      ISurface::SurfacePersistentHandle createNewHandle();

      ModelBasedWorld world;
      std::vector<std::shared_ptr<ModelBasedWorldEntity>> entities;
    public:
      //Default
      WorldFileBasedStateController() = delete;

      WorldFileBasedStateController(
          std::string surfaceDatabasePath,
          std::string surfaceDatabaseName,
          ape::visualization::IVisualizationController* visualizationController);

      // Copy constructor
      WorldFileBasedStateController(const WorldFileBasedStateController&) = default;

      // Move constructor
      WorldFileBasedStateController(WorldFileBasedStateController&&) = default;

      // Copy assignment operator
      WorldFileBasedStateController&
      operator=(const WorldFileBasedStateController&)& = default;

      // Move assignment operator
      WorldFileBasedStateController&
      operator=(WorldFileBasedStateController&&)& = default;

      // Destructor
      virtual ~WorldFileBasedStateController() = default;

      virtual ISurface::SurfacePersistentHandle createSurface(
          std::string name, cv::Mat texture) override;

      virtual bool removeSurface(
          ISurface::SurfacePersistentHandle surfaceHandle) override;

      virtual void persistSurface(
          ISurface::SurfacePersistentHandle surfaceHandle) override;

      virtual std::shared_ptr<ISurface> getSurface(
          ISurface::SurfacePersistentHandle surfaceHandle) override;

      virtual std::vector<
          ISurface::SurfacePersistentHandle> getSurfaceHandles() override;

      virtual IWorld* loadWorld(std::string path) override;

      virtual void update(float timeStep) override;
    };

  }
}

