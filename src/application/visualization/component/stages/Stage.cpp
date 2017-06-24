//
// Created by moritz on 24.06.17.
//

#include "Stage.h"

namespace ape {
  namespace visualization {

    Stage::Stage(AppWindow* appWindow):
      appWindow(appWindow), active(false) {
    }

    void Stage::processInput(glm::vec2 point) {
    }

  }
}