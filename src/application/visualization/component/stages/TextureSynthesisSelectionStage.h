#pragma once
//
// Created by moritz on 05.06.17.
//


#include <cmath>
#include <overlay/Shapes.h>
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

      CustomValueCallback<
          IVisualizationController::OverlayChangeRequestHandler,
          void*>& overlayChangeRequestHandler;

      ValueSignal<glm::vec2> vertex1;
      ValueSignal<glm::vec2> vertex2;

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
              void*>& overlayChangeRequestHandler);

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
    };

  }
}

