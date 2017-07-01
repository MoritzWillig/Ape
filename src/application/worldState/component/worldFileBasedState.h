#pragma once

#include <map>
#include <memory>

#include "worldState.h"
#include "SurfaceImplementation.h"

namespace ape {
  namespace worldState {

    class WorldFileBasedStateController : public IWorldStateController {
    private:
      std::string surfaceDatabasePath;
      std::string surfaceDatabaseName;
    protected:
      void loadSurfaceDatabase();

      std::string getFileNameFromHandle(
          ISurface::SurfacePersistentHandle surfaceHandle);

      std::map<
          ISurface::SurfacePersistentHandle,
          std::shared_ptr<SurfaceImplementation>> surfaces;

      ISurface::SurfacePersistentHandle createNewHandle();
    public:
      //Default
      WorldFileBasedStateController() = delete;

      WorldFileBasedStateController(
          std::string surfaceDatabasePath,
          std::string surfaceDatabaseName);

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

      virtual void update(float timeStep) override;
    };

  }
}

