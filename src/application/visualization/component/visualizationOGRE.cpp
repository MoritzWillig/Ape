#include "visualization.h"

#include "AppWindow.h"
#include "visualizationOGRE.h"

namespace ape {
  namespace visualization {

    OGREVisualizationController::OGREVisualizationController(
        imageProcessing::CameraStream* stream):
        appWindow(new AppWindow()), percent(0.0), ldcStage(appWindow),
        tssStage(appWindow), wsStage(appWindow,overlayChangeRequestHandler),
        stream(stream) {
      overlayChangeRequestHandler.setCallback(nullptr,nullptr);

      appWindow->keyEventHandler.setCallback([](
          void* selfPtr, int key, int scancode, int action, int mods
      ) -> void {
        auto self=(OGREVisualizationController*)selfPtr;
        self->wsStage.processKeyEvent(key,scancode,action,mods);
        self->ldcStage.processKeyEvent(key,scancode,action,mods);
        self->tssStage.processKeyEvent(key,scancode,action,mods);
        self->wsStage.processKeyEvent(key,scancode,action,mods);
      }, this);

      appWindow->mousePositionEventHandler.setCallback([](
          void* selfPtr, double x, double y
      ) -> void {
        auto self=(OGREVisualizationController*)selfPtr;
        self->wsStage.processMousePositionEvent(x,y);
        self->ldcStage.processMousePositionEvent(x,y);
        self->tssStage.processMousePositionEvent(x,y);
        self->wsStage.processMousePositionEvent(x,y);
      }, this);

      appWindow->mouseButtonEventHandler.setCallback([](
          void* selfPtr, int button, int action, int mods
      ) -> void {
        auto self=(OGREVisualizationController*)selfPtr;
        self->wsStage.processMouseButtonEvent(button,action,mods);
        self->ldcStage.processMouseButtonEvent(button,action,mods);
        self->tssStage.processMouseButtonEvent(button,action,mods);
        self->wsStage.processMouseButtonEvent(button,action,mods);
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
      wsStage.update(timeStep);
      tssStage.update(timeStep);
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

  }
}