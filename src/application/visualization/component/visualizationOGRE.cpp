#include <visualModel/OgreVisualModel.h>
#include "visualization.h"

#include "AppWindow.h"
#include "visualizationOGRE.h"

namespace ape {
  namespace visualization {

    OGREVisualizationController::OGREVisualizationController(
        imageProcessing::CameraStream* stream):
        appWindow(new AppWindow()), percent(0.0),
        surfaces(), surfaceNames(), ldcStage(appWindow),
        tssStage(appWindow,overlayChangeRequestHandler,
                 textureGenerationRequestHandler),
        ssStage(appWindow, surfaceSelectionHandler),
        wsStage(appWindow, overlayChangeRequestHandler, ssStage, &surfaceNames),
        stream(stream), viewMatrix(), visualModelIdGenerator(), modelOgreLink(),
        modelSubOgreLink() {
      overlayChangeRequestHandler.setCallback(nullptr,nullptr);

      appWindow->keyEventHandler.setCallback([](
          void* selfPtr, int key, int scancode, int action, int mods
      ) -> void {
        auto self=(OGREVisualizationController*)selfPtr;
        self->wsStage.processKeyEvent(key,scancode,action,mods);
        self->ldcStage.processKeyEvent(key,scancode,action,mods);
        self->tssStage.processKeyEvent(key,scancode,action,mods);
        self->ssStage.processKeyEvent(key,scancode,action,mods);
      }, this);

      appWindow->mousePositionEventHandler.setCallback([](
          void* selfPtr, double x, double y
      ) -> void {
        auto self=(OGREVisualizationController*)selfPtr;
        self->wsStage.processMousePositionEvent(x,y);
        self->ldcStage.processMousePositionEvent(x,y);
        self->tssStage.processMousePositionEvent(x,y);
        self->ssStage.processMousePositionEvent(x,y);
      }, this);

      appWindow->mouseButtonEventHandler.setCallback([](
          void* selfPtr, int button, int action, int mods
      ) -> void {
        auto self=(OGREVisualizationController*)selfPtr;
        self->wsStage.processMouseButtonEvent(button,action,mods);
        self->ldcStage.processMouseButtonEvent(button,action,mods);
        self->tssStage.processMouseButtonEvent(button,action,mods);
        self->ssStage.processMouseButtonEvent(button,action,mods);
      }, this);

      appWindow->entitySelectionEventHandler.setCallback([](
          void* selfPtr,
          Ogre::Entity* entity,
          int subIndex
      ) -> void {
        auto self=(OGREVisualizationController*)selfPtr;

        if (entity==nullptr) {
          self->entitySelectionHandler.callExceptIfNotSet(-1);
          return;
        }

        Ogre::SubEntity* subEnt = entity->getSubEntity(subIndex);

        for (auto it = self->modelSubOgreLink.begin(); it != self->modelSubOgreLink.end(); it++) {
          if (it->second == subEnt) {
            self->entitySelectionHandler.callExceptIfNotSet(
                it->first);
            return;
          }
        }

        throw std::runtime_error("unknown subentity");
      }, this);
    }

    void OGREVisualizationController::setOverlay(Overlay overlay, bool enable) {
      std::string stageTitle;
      switch (overlay) {
        case Overlay::Loading:
          stageTitle="Loading";
          ldcStage.setActive(enable);
          break;
        case Overlay::Menu:
          stageTitle="Menu";
          //FIXME
          //xyzStage.setActive(enable);
          break;
        case Overlay::WorldScreen:
          stageTitle="WorldScreen";
          wsStage.setActive(enable);
          break;
        case Overlay::TextureSynthesisSelection:
          stageTitle="TextureSynthesisSelection";
          tssStage.setActive(enable);
          break;
        case Overlay::SurfaceSelection:
          stageTitle="SurfaceSelection";
          ssStage.setActive(enable);
          break;
      }

      appWindow->setWindowHint(stageTitle);
    }

    bool OGREVisualizationController::getTerminateRequest() {
      return appWindow->isClosed();
    }

    void OGREVisualizationController::update(
        float timeStep) {
      appWindow->update(timeStep, stream, viewMatrix);

      //FIXME all stages should contain an update method ...
      ldcStage.update(timeStep);
      wsStage.update(timeStep);
      tssStage.update(timeStep);
      wsStage.update(timeStep);
    }

    void OGREVisualizationController::setViewTransform(const glm::mat4x4 viewMatrix) {
      this->viewMatrix=viewMatrix;
    }

    void OGREVisualizationController::setProjectionMatrix(const glm::mat3x3 projectionMatrix) {
      appWindow->setProjectionMatrix(projectionMatrix);
    }

    void OGREVisualizationController::setProgress(float percent) {
      auto diff=this->percent-percent;
      ldcStage.update(diff);

      this->percent=percent;
    }

    void OGREVisualizationController::registerSurface(std::string name,
                                                      cv::Mat data) {
      std::string materialName = appWindow->registerTexture(name,data);
      surfaces.emplace(name, materialName);
      surfaceNames.emplace_back(name);
    }

    std::shared_ptr<IVisualModel> OGREVisualizationController::loadModel(
        std::string path) {
      auto model=appWindow->loadModel(path);

      auto visId=visualModelIdGenerator.getNew();
      auto visModel=std::make_shared<OgreVisualModel>(visId);
      modelOgreLink[visId]=model;

      auto ns=model->getNumSubEntities();
      for (unsigned int i=0; i<ns; i++) {
        auto se=model->getSubEntity(i);

        auto visSubId=visualModelIdGenerator.getNew();
        auto visSubModel=std::make_shared<OgreVisualModel>(visSubId);
        modelSubOgreLink[visSubId]=se;

        visModel->addSubEntity(visSubModel);
      }

      return visModel;
    }

    void OGREVisualizationController::setSurface(
        IVisualModel::VisualModelHandle visualHandle,
        std::string surfaceName) {
      auto entity=modelSubOgreLink[visualHandle];
      auto material = appWindow->getTextureName(surfaceName);
      entity->setMaterialName(material->getName());
      /*
      Ogre::SubMesh* subMesh = entity->getMesh()->getSubMesh(subIndex);
      subMesh->setMaterialName(
          "CubeMaterial",
          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
      Ogre::MeshPtr mesh = entity->getMesh();
      mesh->updateMaterialForAllSubMeshes();
      sdafdfsaasdf*/
    }

  }
}