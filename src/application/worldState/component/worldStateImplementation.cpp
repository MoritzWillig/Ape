//
// Created by moritz on 01.07.17.
//

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "worldStateImplementation.h"

namespace ape {
  namespace worldState {

    WorldStateImplementation::WorldStateImplementation(
        std::string surfaceDatabasePath,
        std::string surfaceDatabaseName):
        surfaceDatabasePath(surfaceDatabasePath),
        surfaceDatabaseName(surfaceDatabaseName), surfaces() {
      loadSurfaceDatabase();
    }

    void WorldStateImplementation::loadSurfaceDatabase() {
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

          surfaces.emplace_back(SurfaceImplementation(
              surfaceDatabasePath+getFileNameFromHandle(handle),handle, nameStr));
        }

      } else {
        //could not open database - create a new one ...
        std::cerr<<"Could not open surface database!"<<std::endl;
        std::cerr<<"Proceeding with empty database."<<std::endl;
      }
    }

    std::string WorldStateImplementation::getFileNameFromHandle(
        ISurface::SurfacePersistentHandle surfaceHandle) {
      return std::to_string(surfaceHandle);
    }

    ISurface::SurfacePersistentHandle
    WorldStateImplementation::createSurface(std::string name, cv::Mat texture) {
      //FIXME
      throw std::runtime_error("Not implemented");
    }

    bool WorldStateImplementation::removeSurface(
        ISurface::SurfacePersistentHandle surfaceHandle) {
      //FIXME
      throw std::runtime_error("Not implemented");
    }

    std::vector<
        ISurface::SurfacePersistentHandle>
    WorldStateImplementation::getSurfaceHandles() {
      //FIXME
      throw std::runtime_error("Not implemented");
    }

    void WorldStateImplementation::update(float timeStep) {
    }

    std::shared_ptr<ISurface> WorldStateImplementation::getSurface(
        ISurface::SurfacePersistentHandle surfaceHandle) {
      //FIXME
      throw std::runtime_error("Not implemented");
    }

  }
}