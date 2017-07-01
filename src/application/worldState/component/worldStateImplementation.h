#pragma once

#include "worldState.h"
#include "SurfaceImplementation.h"

namespace ape {
  namespace worldState {

    class WorldStateImplementation : public IWorldStateController {
    private:
      std::string surfaceDatabasePath;
      std::string surfaceDatabaseName;
    protected:
      void loadSurfaceDatabase();

      std::string getFileNameFromHandle(
          ISurface::SurfacePersistentHandle surfaceHandle);

      std::vector<SurfaceImplementation> surfaces;
    public:
      //Default
      WorldStateImplementation() = delete;

      WorldStateImplementation(
          std::string surfaceDatabasePath,
          std::string surfaceDatabaseName);

      // Copy constructor
      WorldStateImplementation(const WorldStateImplementation&) = default;

      // Move constructor
      WorldStateImplementation(WorldStateImplementation&&) = default;

      // Copy assignment operator
      WorldStateImplementation&
      operator=(const WorldStateImplementation&)& = default;

      // Move assignment operator
      WorldStateImplementation&
      operator=(WorldStateImplementation&&)& = default;

      // Destructor
      virtual ~WorldStateImplementation() = default;

      virtual ISurface::SurfacePersistentHandle createSurface(
          std::string name, cv::Mat texture) override;

      virtual bool removeSurface(
          ISurface::SurfacePersistentHandle surfaceHandle) override;

      virtual std::shared_ptr<ISurface> getSurface(
          ISurface::SurfacePersistentHandle surfaceHandle) override;

      virtual std::vector<
          ISurface::SurfacePersistentHandle> getSurfaceHandles() override;

      virtual void update(float timeStep) override;
    };

  }
}

