#pragma once

#include <memory>

#include <imageProcessing/CameraStream.h>
#include "glm/glm.hpp"
#include <worldState/Surface.h>
#include <common/callbacks/CustomValueCallback.h>
#include <visualization/VisualModel.h>

namespace ape {
  namespace visualization {

    class IVisualizationController {
    private:
    protected:
    public:
      IVisualizationController();

      enum class Overlay {
        Loading,
        Menu,
        WorldScreen,
        TextureSynthesisSelection,
        SurfaceSelection
      };

      virtual void setOverlay(Overlay overlay, bool enable) = 0;

      virtual void update(float timeStep) = 0;

      virtual bool getTerminateRequest() = 0;

      virtual void setViewTransform(const glm::mat4x4 viewMatrix) = 0;

      virtual void setProjectionMatrix(const glm::mat3x3 projectionMatrix) = 0;

      virtual void setProgress(float d) = 0;

      virtual void registerSurface(std::string name, cv::Mat data) = 0;

      typedef void (*OverlayChangeRequestHandler)(void* custom, Overlay overlay);

      typedef void (*TextureGenerationFinishedHandler)(void* custom, int textureId);

      typedef void (*TextureGenerationRequestHandler)(void* custom,
        glm::vec2 v1, glm::vec2 v2, CustomValueCallback<
          TextureGenerationFinishedHandler,void*>* generationFinishedHandler);

      CustomValueCallback<
          IVisualizationController::OverlayChangeRequestHandler,
          void*> overlayChangeRequestHandler;

      CustomValueCallback<
          IVisualizationController::TextureGenerationRequestHandler,
          void*> textureGenerationRequestHandler;

      enum class SurfaceSelectionAction {
        SELECT_TEMPORARY,
        SELECT_PERMANENT,
        RESET
      };

      typedef void (*SurfaceSelectionHandler)
          (void* custom, SurfaceSelectionAction action,
           ape::worldState::ISurface* surface);

      CustomValueCallback<
          IVisualizationController::SurfaceSelectionHandler,
          void*> surfaceSelectionHandler;

      virtual std::shared_ptr<IVisualModel> loadModel(std::string path) = 0;


      static std::shared_ptr<IVisualizationController> createInstance(
          imageProcessing::CameraStream* stream
      );
    };
  }
}