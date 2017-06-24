//
// Created by moritz on 05.06.17.
//

#include <math.h>

#include <glm/vec2.hpp>
#include <overlay/OgreShapes.h>
#include "WorldScreenStage.h"


namespace ape {
  namespace visualization {

    WorldScreenStage::WorldScreenStage(AppWindow* appWindow):
        Stage(appWindow), overlay() {
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

  }
}