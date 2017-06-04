#pragma once


#include "visualization.h"

#include "AppWindow.h"

namespace ape {
  namespace visualization {

    class OGREVisualizationController: public IVisualizationController {
    private:
      imageProcessing::CameraStream* stream;
      AppWindow* scene;

      glm::mat4x4 viewMatrix;
    protected:
    public:
      OGREVisualizationController() = delete;

      OGREVisualizationController(imageProcessing::CameraStream* stream);

      virtual void startDisplay() override;

      virtual void setOverlay(Overlay overlay) override;

      virtual void update(float timeStep) override;

      virtual bool getTerminateRequest() override;

      virtual void setViewTransform(const glm::mat4x4 viewMatrix) override;

      virtual void setProgress(float percent) override;
    };
  }
}