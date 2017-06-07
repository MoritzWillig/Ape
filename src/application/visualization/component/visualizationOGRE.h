#pragma once


#include "visualization.h"

#include "AppWindow.h"

#include "stages/LoadingControllerStage.h"

namespace ape {
  namespace visualization {

    class OGREVisualizationController: public IVisualizationController {
    private:
      AppWindow* appWindow;

      float percent;
      LoadingControllerStage ldcStage;

      imageProcessing::CameraStream* stream;

      glm::mat4x4 viewMatrix;
    protected:
    public:
      OGREVisualizationController() = delete;

      OGREVisualizationController(imageProcessing::CameraStream* stream);

      virtual void setOverlay(Overlay overlay) override;

      virtual void update(float timeStep) override;

      virtual bool getTerminateRequest() override;

      virtual void setViewTransform(const glm::mat4x4 viewMatrix) override;

      virtual void setProgress(float percent) override;
    };
  }
}