#include <fstream>
#include <iostream>
#include <sstream>
#include "worldFileBasedState.h"

namespace ape {
  namespace worldState {

    WorldFileBasedStateController::WorldFileBasedStateController(
        std::string surfaceDatabasePath,
        std::string surfaceDatabaseName):
        surfaceDatabasePath(surfaceDatabasePath),
        surfaceDatabaseName(surfaceDatabaseName), surfaces() {
      loadSurfaceDatabase();
    }

    void WorldFileBasedStateController::loadSurfaceDatabase() {
      //TODO c++17 defines a preferred delimiter
      std::fstream fs(surfaceDatabasePath+"/"+surfaceDatabaseName);

      if (fs) {
        std::string line;
        std::stringstream lineStream;

        while (std::getline(fs,line)) {
          lineStream.clear();
          lineStream<<line;

          std::string handleStr;
          std::string nameStr;
          std::getline(lineStream,handleStr,'|');
          std::getline(lineStream,nameStr);

          ISurface::SurfacePersistentHandle handle=std::stoi(handleStr);

          surfaces.emplace(
              handle,
              std::make_shared<SurfaceImplementation>(
                  surfaceDatabasePath+getFileNameFromHandle(handle),
                  handle,
                  nameStr
              )
          );
        }
      } else {
        //could not open database - create a new one ...
        std::cerr<<"Could not open surface database!"<<std::endl;
        std::cerr<<"Proceeding with empty database."<<std::endl;
      }
    }

    std::string WorldFileBasedStateController::getFileNameFromHandle(
        ISurface::SurfacePersistentHandle surfaceHandle) {
      return std::to_string(surfaceHandle)+".jpg";
    }

    ISurface::SurfacePersistentHandle
    WorldFileBasedStateController::createSurface(std::string name, cv::Mat texture) {
      ISurface::SurfacePersistentHandle handle=createNewHandle();
      surfaces.emplace(
          handle,
          std::make_shared<SurfaceImplementation>(
              surfaceDatabasePath+getFileNameFromHandle(handle),
              handle,
              name
          )
      );

      return handle;
    }

    bool WorldFileBasedStateController::removeSurface(
        ISurface::SurfacePersistentHandle surfaceHandle) {
      surfaces.erase(surfaces.find(surfaceHandle));
      return true;
    }

    void WorldFileBasedStateController::persistSurface(
        ISurface::SurfacePersistentHandle surfaceHandle) {
      //FIXME
      throw std::runtime_error("Not implemented");
    }

    std::vector<
        ISurface::SurfacePersistentHandle>
    WorldFileBasedStateController::getSurfaceHandles() {
      std::vector<ISurface::SurfacePersistentHandle> handles;

      for (const auto surface: surfaces) {
        handles.push_back(surface.first);
      }

      return handles;
    }

    void WorldFileBasedStateController::update(float timeStep) {
    }

    std::shared_ptr<ISurface> WorldFileBasedStateController::getSurface(
        ISurface::SurfacePersistentHandle surfaceHandle) {
      auto surfaceIt=surfaces.find(surfaceHandle);
      if (surfaceIt==surfaces.end()) {
        throw std::runtime_error("Invalid surface handle");
      }

      return surfaceIt->second;
    }

    ISurface::SurfacePersistentHandle
    WorldFileBasedStateController::createNewHandle() {
      ISurface::SurfacePersistentHandle currentHandle=0;
      while (surfaces.find(currentHandle)!=surfaces.end()) {
        currentHandle++;
      }
      return currentHandle;
    }

  }
}