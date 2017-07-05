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
        SurfaceSelectionStage& surfaceSelectionStage):
        Stage(appWindow), overlay(),
        overlayChangeRequestHandler(overlayChangeRequestHandler),
        surfaceSelectionStage(surfaceSelectionStage) {
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
          surfaceSelectionStage.dictPosition=glm::vec2(-1.0,-0.2);
          surfaceSelectionStage.dictSize=glm::vec2(2.0,0.8);
          surfaceSelectionStage.surfaceButtonSize=glm::vec2(0.20,0.25);
          surfaceSelectionStage.surfaceButtonPadding=0.025;
          //FIXME ...
          surfaceSelectionStage.surfaces.clear();
          surfaceSelectionStage.surfaces.emplace_back("X");
          surfaceSelectionStage.surfaces.emplace_back("Y");
          surfaceSelectionStage.surfaces.emplace_back("Z");
          surfaceSelectionStage.surfaces.emplace_back("W");
          surfaceSelectionStage.surfaces.emplace_back("K");
          surfaceSelectionStage.surfaces.emplace_back("H");
          surfaceSelectionStage.surfaces.emplace_back("Q");
          surfaceSelectionStage.updateShapes();
          surfaceSelectionStage.setActive(true);
        default:
          break;
      }
    }

  }
}