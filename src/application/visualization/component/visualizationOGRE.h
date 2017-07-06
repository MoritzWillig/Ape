#pragma once


#include "visualization.h"

#include "AppWindow.h"

#include "stages/LoadingControllerStage.h"
#include "stages/TextureSynthesisSelectionStage.h"
#include "stages/WorldScreenStage.h"
#include "stages/SurfaceSelectionStage.h"

namespace ape {
  namespace visualization {

    class OGREVisualizationController: public IVisualizationController {
    private:
      AppWindow* appWindow;

      float percent;

      /**
       * key  : surface name known to the interface
       * value: material name generated by appWindow
       */
      std::map<std::string, std::string> surfaces;
      std::vector<std::string> surfaceNames;

      LoadingControllerStage ldcStage;
      TextureSynthesisSelectionStage tssStage;
      SurfaceSelectionStage ssStage;
      WorldScreenStage wsStage;

      imageProcessing::CameraStream* stream;

      glm::mat4x4 viewMatrix;
    protected:
    public:
      OGREVisualizationController() = delete;

      OGREVisualizationController(imageProcessing::CameraStream* stream);

      virtual void setOverlay(Overlay overlay, bool enable) override;

      virtual void update(float timeStep) override;

      virtual bool getTerminateRequest() override;

      virtual void setViewTransform(const glm::mat4x4 viewMatrix) override;

      virtual void setProjectionMatrix(const glm::mat3x3 projectionMatrix) override;

      virtual void setProgress(float percent) override;

      virtual void registerSurface(std::string name, cv::Mat data) override;
    };
  }
}