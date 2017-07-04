
#include <overlay/OgreButton.h>
#include "SurfaceSelectionStage.h"


namespace ape {
  namespace visualization {

    SurfaceSelectionStage::SurfaceSelectionStage(
        AppWindow* appWindow,
        CustomValueCallback<
            IVisualizationController::SurfaceSelectionHandler,
            void*>& surfaceSelectionHandler):
        Stage(appWindow), overlay(), selectedItem(nullptr),
        hoveredItem(nullptr), scrollPosition(0),
        surfaceSelectionHandler(surfaceSelectionHandler),
        lastMousePosition() {

      //TODO do not hardcode surface patches but implement updateShapes()
      //updateShapes();

      auto dictButton=std::make_shared<shapes::OgreButton>(
          appWindow, -0.95,0.95,0.3,0.2,Ogre::ColourValue(0.8f,0.0f,0.0f));
      overlay.childs.emplace_back(dictButton);

      setActive(false);
    }

    void SurfaceSelectionStage::setActive(bool active) {
      this->active=active;

      if (this->active) {
        setScrollPosition(0);
        selectedItem=nullptr;
        hoveredItem=nullptr;

        overlay.setVisible(true);
      } else {
        overlay.setVisible(false);
      }
    }

    void SurfaceSelectionStage::update(float delta) {
      if (!active) {
        return;
      }
    }

    void SurfaceSelectionStage::setSelection(
        ape::worldState::ISurface* surface) {
      selectedItem=surface;
      //TODO update visual ...
      //TODO send SELECT_PERMANENT request
    }

    void SurfaceSelectionStagesetSelection(ape::worldState::ISurface* surface);

    void SurfaceSelectionStage::processKeyEvent(
        int key, int scancode, int action, int mods) {
      Stage::processKeyEvent(key, scancode, action, mods);

      if (!active) {
        return;
      }

      switch (key) {
        case GLFW_KEY_ESCAPE:
          if (action==GLFW_RELEASE) {
            setActive(false);
          }
          break;
        default:
          break;
      }
    }

    void SurfaceSelectionStage::processMousePositionEvent(double x,
                                                                   double y) {
      Stage::processMousePositionEvent(x, y);

      lastMousePosition.x=(float)x;
      lastMousePosition.y=(float)y;

      if (!active) {
        return;
      }

      //TODO check 'hover' and send SELECT_TEMPORARY request

    }

    void SurfaceSelectionStage::processMouseButtonEvent(int button,
                                                                 int action,
                                                                 int mods) {
      Stage::processMouseButtonEvent(button, action, mods);

      if (!active) {
        return;
      }

      if ((button==GLFW_MOUSE_BUTTON_LEFT) && (action==GLFW_RELEASE)) {
        //send selection permanent, change "selected"
        throw std::runtime_error("Not implemented");
      }
    }

    void SurfaceSelectionStage::setScrollPosition(int position) {
      //FIXME implement ...
    }

  }
}