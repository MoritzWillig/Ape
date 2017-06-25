//
// Created by moritz on 24.06.17.
//

#include "Stage.h"

namespace ape {
  namespace visualization {

    Stage::Stage(AppWindow* appWindow):
      appWindow(appWindow), active(false) {
    }

    void Stage::processKeyEvent(
        int key, int scancode, int action, int mods) {
    }

    void Stage::processMousePositionEvent(
        double x, double y) {
    }

    void Stage::processMouseButtonEvent(
        int button, int action, int mods) {
    }

  }
}