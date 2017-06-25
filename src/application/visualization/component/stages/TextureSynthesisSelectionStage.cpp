//
// Created by moritz on 05.06.17.
//

#include "TextureSynthesisSelectionStage.h"


namespace ape {
  namespace visualization {

    TextureSynthesisSelectionStage::TextureSynthesisSelectionStage(
        AppWindow* appWindow,
        CustomValueCallback<
            IVisualizationController::OverlayChangeRequestHandler,
            void*>& overlayChangeRequestHandler):
        Stage(appWindow), overlay(),
        overlayChangeRequestHandler(overlayChangeRequestHandler),
        vertex1(), vertex2(), selectionState(SelectionState::CapturingP1) {
      setActive(false);
    }

    void TextureSynthesisSelectionStage::setActive(bool active) {
      this->active=active;

      if (this->active) {
        vertex1.reset();
        vertex2.reset();
        selectionState = SelectionState::CapturingP1;

        overlay.setVisible(true);
      } else {
        overlay.setVisible(false);
      }
    }

    void TextureSynthesisSelectionStage::update(float delta) {
      if (!active) {
        return;
      }
    }

    void TextureSynthesisSelectionStage::processKeyEvent(
        int key, int scancode, int action, int mods) {
      Stage::processKeyEvent(key, scancode, action, mods);

      if (!active) {
        return;
      }

      switch (key) {
        case GLFW_KEY_ESCAPE:
          overlayChangeRequestHandler.callExceptIfNotSet(
              IVisualizationController::Overlay::WorldScreen);
          break;
        default:
          break;
      }
    }

    void TextureSynthesisSelectionStage::performStateTransition(
        TextureSynthesisSelectionStage::SelectionState newSelectionState) {
      //disable old state elements
      switch (selectionState) {
        case SelectionState::CapturingP1:break;
        case SelectionState::CapturingP2:break;
        case SelectionState::Waiting:break;
        case SelectionState::Generating:break;
        case SelectionState::Confirmation:break;
      }
      selectionState=newSelectionState;
      //add new state elements
      switch (selectionState) {
        case SelectionState::CapturingP1:
          break;
        case SelectionState::CapturingP2:
          //FIXME draw currently selected area
          break;
        case SelectionState::Waiting:
          //FIXME add currently selected area
          //FIXME add edit handlers
          break;
        case SelectionState::Generating: {
          //FIXME start generation

          //FIXME show loading bar ...
        }
          break;
        case SelectionState::Confirmation:
          //FIXME show result - "name" field + "ok"/"_abort" button
          break;
      }
    }

  }
}