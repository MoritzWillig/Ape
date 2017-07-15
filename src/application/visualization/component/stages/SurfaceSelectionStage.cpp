
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
        lastMousePosition(), dictPosition(0,0), dictSize(1.0,1.0),
        surfaceButtonSize(0.09,0.09), surfaceButtonPadding(0.05),
        surfaces() {
      updateShapes();
      setActive(false);
    }

    void SurfaceSelectionStage::setActive(bool active) {
      this->active=active;

      if (this->active) {
        setScrollPosition(0);
        selectedItem=nullptr;
        hoveredItem=nullptr;

        updateShapes();

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

    void SurfaceSelectionStage::updateShapes() {
      overlay.childs.clear();

      glm::vec2 patchPosition(0.0,-surfaceButtonPadding);

      for (auto const surface: surfaces) {
        if (patchPosition.x+
                surfaceButtonSize.x+
                2*surfaceButtonPadding>dictSize.x) {
          patchPosition.x=0.0;
          patchPosition.y-=surfaceButtonSize.y+2*surfaceButtonPadding;
        }
        patchPosition.x+=surfaceButtonPadding;

        auto dictSurfaceButton=std::make_shared<shapes::OgreButton>(
            appWindow,
            dictPosition.x+patchPosition.x, dictPosition.y+patchPosition.y,
            surfaceButtonSize.x, surfaceButtonSize.y,
            Ogre::ColourValue(1.0f,1.0f,1.0f));
        dictSurfaceButton->textureName.setValue(
            appWindow->getTextureName(surface)->getName());
        //TODO update should be triggered when setting texture
        //or be taken care of by a handle
        dictSurfaceButton->updateOgreObject();
        overlay.childs.emplace_back(dictSurfaceButton);

        //TODO update visual if surface is hovered or selected

        patchPosition.x+=surfaceButtonSize.x+surfaceButtonPadding;
      }
    }

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
        case GLFW_KEY_1:
          //FIXME
          surfaceSelectionHandler.callExceptIfNotSet(
              IVisualizationController::SurfaceSelectionAction::SELECT_PERMANENT,
              surfaces[0]
          );
          break;
        case GLFW_KEY_2:
          //FIXME
          surfaceSelectionHandler.callExceptIfNotSet(
              IVisualizationController::SurfaceSelectionAction::SELECT_PERMANENT,
              surfaces[1]
          );
          break;
        case GLFW_KEY_3:
          //FIXME
          surfaceSelectionHandler.callExceptIfNotSet(
              IVisualizationController::SurfaceSelectionAction::SELECT_PERMANENT,
              surfaces[2]
          );
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
        //TODO send selection permanent, change "selected"
      }
    }

    void SurfaceSelectionStage::setScrollPosition(int position) {
      //FIXME implement ...
    }

  }
}