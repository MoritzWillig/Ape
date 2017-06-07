#include "visualization.h"

#include "AppWindow.h"
#include "visualizationOGRE.h"

namespace ape {
  namespace visualization {

    OGREVisualizationController::OGREVisualizationController(
        imageProcessing::CameraStream* stream):
        appWindow(new AppWindow()), percent(0.0), ldcStage(appWindow), stream(stream) {
    }

    void OGREVisualizationController::setOverlay(Overlay overlay) {
      switch (overlay) {
        case Overlay::Loading:
          ldcStage.setActive(true);
          break;
        case Overlay::Menu:
          ldcStage.setActive(false);
          break;
        case Overlay::WorldScreen:
          ldcStage.setActive(false);
          break;
      }
    }

    bool OGREVisualizationController::getTerminateRequest() {
      return appWindow->isClosed();
    }

    void OGREVisualizationController::update(
        float timeStep) {
      appWindow->update(timeStep, stream, viewMatrix);
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