//
// Created by moritz on 05.06.17.
//

#include "TextureSynthesisSelectionStage.h"


namespace ape {
  namespace visualization {

    TextureSynthesisSelectionStage::TextureSynthesisSelectionStage(AppWindow* appWindow):
        Stage(appWindow), overlay() {
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

  }
}