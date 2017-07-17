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
        ape::imageProcessing::IImageProcessingController* ipController,
        CustomValueCallback<
            IVisualizationController::OverlayChangeRequestHandler,
            void*>& overlayChangeRequestHandler,
        SurfaceSelectionStage& surfaceSelectionStage,
        std::vector<std::string>* surfaceNames):
        Stage(appWindow), lastMousePosition(-1,-1), ipController(ipController),
        overlay(), texSynthButton(),
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

      closeButton=std::make_shared<shapes::OgreButton>(
          appWindow,
          -0.99, 0.99, 0.2, 0.2,
          Ogre::ColourValue(1.0f,1.0f,1.0f));
      closeButton->textureName.setValue("close_ico");
      closeButton->updateOgreObject();
      overlay.childs.emplace_back(closeButton);

      colorBalancingButton=std::make_shared<shapes::OgreButton>(
          appWindow,
          0.55, 0.99, 0.2, 0.2,
          Ogre::ColourValue(1.0f,1.0f,1.0f));
      colorBalancingButton->textureName.setValue("texSelect_ico");
      colorBalancingButton->updateOgreObject();
      overlay.childs.emplace_back(colorBalancingButton);



      setActive(false);
    }

    bool WorldScreenStage::isActive() {
      return active;
    }

    void WorldScreenStage::setActive(bool active) {
      this->active=active;
      overlay.setVisible(active);
      appWindow->enableHitTest(active);

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
          break;
        case GLFW_KEY_ESCAPE:
          this->appWindow->closeWindow();
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
                                                        int mods, bool& cancel) {
      Stage::processMouseButtonEvent(button, action, mods, cancel);

      if (!active) {
        return;
      }

      if ((button==GLFW_MOUSE_BUTTON_LEFT) && (action==GLFW_RELEASE)) {
        if (texSynthButton->hit(lastMousePosition)) {
          overlayChangeRequestHandler.callExceptIfNotSet(
              IVisualizationController::Overlay::TextureSynthesisSelection);
        }

        if (closeButton->hit(lastMousePosition)) {
          this->appWindow->closeWindow();
        }

        if (colorBalancingButton->hit(lastMousePosition)) {
          ipController->setWhiteBalance(!appWindow->getColorBalancing());
          appWindow->setColorBalancing(!appWindow->getColorBalancing());

        }
      }
    }

  }
}