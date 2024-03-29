#pragma once
//
// Created by moritz on 05.06.17.
//


#include <cmath>
#include <overlay/OgreButton.h>
#include <OGRE/OgrePolygon.h>
#include <AppWindow.h>
#include <common/signals/Signal.h>

#include "Stage.h"
#include "../visualization.h"
//FIXME check correct import
#include "../../../imageProcessing/component/imageProcessing.h"

namespace ape {
  namespace visualization {

    class TextureSynthesisSelectionStage: public Stage {
    private:
      shapes::Container overlay;
      std::shared_ptr<ape::visualization::shapes::OgreButton> abortButton;
      std::shared_ptr<ape::visualization::shapes::OgreButton> selectionButton;
      std::shared_ptr<ape::visualization::shapes::OgreButton> okButton;

      CustomValueCallback<
          IVisualizationController::OverlayChangeRequestHandler,
          void*>& overlayChangeRequestHandler;

      CustomValueCallback<
          IVisualizationController::TextureGenerationRequestHandler,
          void*>& textureGenerationRequestHandler;

      ValueSignal<glm::vec2> vertex1;
      ValueSignal<glm::vec2> vertex2;

      glm::vec2 lastMousePosition;

      CustomValueCallback<
          IVisualizationController::TextureGenerationFinishedHandler,
          void*
      > generationFinishedHandler;

      enum class SelectionState {
        CapturingP1,
        CapturingP2,
        Waiting,
        Generating,
        Confirmation
      } selectionState;

      void performStateTransition(SelectionState newSelectionState);
    protected:
    public:
      //Default
      TextureSynthesisSelectionStage() = delete;

      TextureSynthesisSelectionStage(
          AppWindow* appWindow,
          CustomValueCallback<
              IVisualizationController::OverlayChangeRequestHandler,
              void*>& overlayChangeRequestHandler,
          CustomValueCallback<
              IVisualizationController::TextureGenerationRequestHandler,
              void*>& textureGenerationRequestHandler);

      // Copy constructor
      TextureSynthesisSelectionStage(const TextureSynthesisSelectionStage&) = default;

      // Move constructor
      TextureSynthesisSelectionStage(TextureSynthesisSelectionStage&&) = default;

      // Copy assignment operator
      TextureSynthesisSelectionStage&
      operator=(const TextureSynthesisSelectionStage&)& = default;

      // Move assignment operator
      TextureSynthesisSelectionStage&
      operator=(TextureSynthesisSelectionStage&&)& = default;

      // Destructor
      virtual ~TextureSynthesisSelectionStage() = default;

      void setActive(bool active);

      void update(float delta);

      virtual void processKeyEvent(
          int key, int scancode, int action, int mods) override;
      virtual void processMousePositionEvent(
          double x, double y) override;
      virtual void processMouseButtonEvent(
          int button, int action, int mods, bool& cancel) override;
    };

  }
}

