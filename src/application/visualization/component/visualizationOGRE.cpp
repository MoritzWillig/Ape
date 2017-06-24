#include "visualization.h"

#include "AppWindow.h"
#include "visualizationOGRE.h"

namespace ape {
  namespace visualization {

    OGREVisualizationController::OGREVisualizationController(
        imageProcessing::CameraStream* stream):
        appWindow(new AppWindow()), percent(0.0), ldcStage(appWindow),
        tssStage(appWindow), wsStage(appWindow), stream(stream) {
      overlayChangeRequestHandler.setCallback(nullptr,nullptr);

      appWindow->keyEventHandler.setCallback([](
          void* selfPtr, int key, int scancode, int action, int mods
      ) -> void {
        auto self=(OGREVisualizationController*)selfPtr;
        switch (key) {
          case 32:
            self->overlayChangeRequestHandler.callExceptIfNotSet(
                Overlay::WorldScreen);
            break;
          default:
            break;
        }
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

    void OGREVisualizationController::setProgress(float percent) {
      auto diff=this->percent-percent;
      ldcStage.update(diff);

      this->percent=percent;
    }

  }
}