#include <fstream>
#include <iostream>
#include <sstream>
#include "worldFileBasedState.h"
#include <model/ModelBasedWorldEntity.h>

namespace ape {
  namespace worldState {

    WorldFileBasedStateController::WorldFileBasedStateController(
        std::string surfaceDatabasePath,
        std::string surfaceDatabaseName,
        ape::visualization::IVisualizationController* visualizationController):
        surfaceDatabasePath(surfaceDatabasePath),
        surfaceDatabaseName(surfaceDatabaseName), entityHandleGenerator(),
        visController(visualizationController), surfaces(), world(),
        entities() {
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
                  surfaceDatabasePath+"/"+getFileNameFromHandle(handle),
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

      //load existing surfaces into visualization controller
      auto surfaceHandles=getSurfaceHandles();
      for (const auto handle: surfaceHandles) {
        auto surface = getSurface(handle);
        visController->registerSurface(surface->getName(),surface->getTexture());
      }
    }

    std::string WorldFileBasedStateController::getFileNameFromHandle(
        ISurface::SurfacePersistentHandle surfaceHandle) {
      return std::to_string(surfaceHandle)+".jpg";
    }

    ISurface::SurfacePersistentHandle
    WorldFileBasedStateController::createSurface(std::string name, cv::Mat texture) {
      ISurface::SurfacePersistentHandle handle=createNewHandle();
      auto surface=std::make_shared<SurfaceImplementation>(
          surfaceDatabasePath+getFileNameFromHandle(handle),
          handle,
          name
      );
      surface->setTexture(texture);
      surfaces.emplace(
          handle,
          surface
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

    IWorld* WorldFileBasedStateController::loadWorld(std::string path) {
      auto model=visController->loadModel(path);

      world=ModelBasedWorld();

      auto mEntities=model->getSubEntities();
      for (auto entity=mEntities->cbegin(); entity!=mEntities->cend(); entity++) {
        auto entityPtr=std::make_shared<ModelBasedWorldEntity>(
            (*entity)->getHandle(),
            entityHandleGenerator.getNew()
        );
        entityPtr->surfaceChangedHandler.setCallback([](
            void* custom,
            ModelBasedWorldEntity* entity
        ) {
          auto self=(WorldFileBasedStateController*)custom;

          //set new surface in visual representation
          self->visController->setSurface(
            entity->getVisualHandle(),
            self->surfaces[entity->getSurface()]->getName());
        }, this);

        entities.emplace_back(entityPtr);

        world.addEntity(entities.back().get());
      }
      return &world;
    }

    void WorldFileBasedStateController::setSurface(
        visualization::IVisualModel::VisualModelHandle visualModelHandle,
        ISurface::SurfacePersistentHandle surfaceHandle) {
      for (auto it=entities.begin(); it!=entities.end(); it++) {
        auto handle=it->get()->getVisualHandle();
        if (handle==visualModelHandle) {
          //set surface to model based entity
          it->get()->setSurface(surfaceHandle);
          return;
        }
      }
      throw std::runtime_error("entity not found");
    }

    ISurface::SurfacePersistentHandle
    WorldFileBasedStateController::getSurfaceByName(std::string name) {
      for (auto it=surfaces.begin(); it!=surfaces.end(); it++) {
        if (it->second->getName()==name) {
          return it->first;
        }
      }
    }

  }
}