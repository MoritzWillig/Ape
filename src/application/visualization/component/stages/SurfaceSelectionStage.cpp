
#include "SurfaceSelectionStage.h"


namespace ape {
  namespace visualization {

    SurfaceSelectionStage::SurfaceSelectionStage(
        IVisualizationController* visController,
        AppWindow* appWindow,
        CustomValueCallback<
            IVisualizationController::SurfaceSelectionHandler,
            void*>& surfaceSelectionHandler):
        visController(visController), Stage(appWindow), overlay(), buttons(),
        selectedItem(nullptr), hoveredItem(nullptr), scrollPosition(0),
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

        //update surfaces from vis controller
        auto surfaceNames=visController->getSurfaceNames();
        surfaces.clear();
        for (const auto surfaceName: *surfaceNames) {
          surfaces.emplace_back(surfaceName);
        }
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
      buttons.clear();

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

        buttons.emplace_back(dictSurfaceButton);

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
        case GLFW_KEY_2:
        case GLFW_KEY_3:
        case GLFW_KEY_4:
        case GLFW_KEY_5:
        case GLFW_KEY_6:
        case GLFW_KEY_7:
        case GLFW_KEY_8:
        case GLFW_KEY_9:
          if (action==GLFW_RELEASE) {
            auto idx = key - GLFW_KEY_1;
            if (idx < visController->getSurfaceNames()->size()) {
              surfaceSelectionHandler.callExceptIfNotSet(
                  IVisualizationController::SurfaceSelectionAction::SELECT_PERMANENT,
                  surfaces[idx]
              );
            }
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
        for (auto i=0; i<buttons.size(); i++) {
          if (buttons[i]->hit(lastMousePosition)) {
            surfaceSelectionHandler.callExceptIfNotSet(
                IVisualizationController::SurfaceSelectionAction::SELECT_PERMANENT,
                surfaces[i]
            );
          }
        }
      }
    }

    void SurfaceSelectionStage::setScrollPosition(int position) {
      //FIXME implement ...
    }

  }
}