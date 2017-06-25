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
            void*>& overlayChangeRequestHandler):
        Stage(appWindow), overlay(),
        overlayChangeRequestHandler(overlayChangeRequestHandler) {
      std::vector<glm::vec2> circle;
      auto circlePosX=0.875;
      auto circlePosY=0.875;
      auto r=0.10;
      int numPts=20;

      circle.emplace_back(circlePosX,circlePosY);
      for (auto i=0; i<numPts+1; i++) {
        circle.emplace_back(
            circlePosX+(std::cos((i/(float)numPts)*M_PI*2.0)*r),
            circlePosY+(std::sin((i/(float)numPts)*M_PI*2.0)*r)
        );
      }

      //texture synthesis button
      auto texSynth=std::make_shared<shapes::OgrePolygon2D>(
          appWindow, circle, Ogre::ColourValue(0.8f,0.0f,0.0f));
      overlay.childs.emplace_back(texSynth);

      setActive(false);
    }

    void WorldScreenStage::setActive(bool active) {
      this->active=active;
      overlay.setVisible(active);
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
        default:
          break;
      }
    }

  }
}