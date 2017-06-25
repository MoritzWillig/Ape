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
        overlayChangeRequestHandler(overlayChangeRequestHandler) {
      setActive(false);
    }

    void TextureSynthesisSelectionStage::setActive(bool active) {
      this->active=active;

      //if active -> capture(?) frame & show overlay
    }

    void TextureSynthesisSelectionStage::update(float delta) {
      if (!active) {
        return;
      }

      //FIXME
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

  }
}