#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/vec2.hpp>
#include <overlay/OgreShapes.h>
#include "WorldScreenStage.h"
#include "../visualization.h"


namespace ape {
  namespace visualization {

    WorldScreenStage::WorldScreenStage(
        AppWindow* appWindow,
        CustomValueCallback<
            IVisualizationController::OverlayChangeRequestHandler,
            void*>& overlayChangeRequestHandler,
        SurfaceSelectionStage& surfaceSelectionStage,
        std::vector<std::string>* surfaceNames):
        lastMousePosition(-1,-1), Stage(appWindow), overlay(), texSynthButton(),
        overlayChangeRequestHandler(overlayChangeRequestHandler),
        surfaceSelectionStage(surfaceSelectionStage),
        surfaceNames(surfaceNames) {
      auto circlePosX=0.875;
      auto circlePosY=0.875;
      auto r=0.10;
      int numPts=20;

      //texture synthesis button
      texSynthButton=std::make_shared<shapes::OgreButton>(
          appWindow,
          circlePosX-r, circlePosY+r, r*2.0, r*2.0,
          Ogre::ColourValue(1.0f,1.0f,1.0f));
      texSynthButton->textureName.setValue("texSelect_ico");
      texSynthButton->updateOgreObject();
      overlay.childs.emplace_back(texSynthButton);

      setActive(false);
    }

    void WorldScreenStage::setActive(bool active) {
      this->active=active;
      overlay.setVisible(active);

      //if we leave this stage, we disable all "sub-stages"
      if (!active) {
        surfaceSelectionStage.setActive(false);
      }
    }

    void WorldScreenStage::update(float delta) {
      if (!active) {
        return;
      }
    }

    void WorldScreenStage::processKeyEvent(
        int key, int scancode, int action, int mods) {
      Stage::processKeyEvent(key, scancode, action, mods);

      if (!active) {
        return;
      }

      switch (key) {
        case GLFW_KEY_SPACE:
          overlayChangeRequestHandler.callExceptIfNotSet(
              IVisualizationController::Overlay::TextureSynthesisSelection);
          break;
        case GLFW_KEY_M:
          overlayChangeRequestHandler.callExceptIfNotSet(
              IVisualizationController::Overlay::Menu);
          break;
        case GLFW_KEY_S:
          surfaceSelectionStage.setActive(true);
        default:
          break;
      }
    }

    void WorldScreenStage::processMousePositionEvent(double x, double y) {
      Stage::processMousePositionEvent(x, y);

      lastMousePosition.x=(float)x;
      lastMousePosition.y=(float)y;

      if (!active) {
        return;
      }

    }

    void WorldScreenStage::processMouseButtonEvent(int button,
                                                        int action,
                                                        int mods) {
      Stage::processMouseButtonEvent(button, action, mods);

      if (!active) {
        return;
      }

      if ((button==GLFW_MOUSE_BUTTON_LEFT) && (action==GLFW_RELEASE)) {
        if (texSynthButton->hit(lastMousePosition)) {
          overlayChangeRequestHandler.callExceptIfNotSet(
              IVisualizationController::Overlay::TextureSynthesisSelection);
        }
      }
    }

  }
}