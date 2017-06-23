//
// Created by moritz on 05.06.17.
//

#include <glm/vec2.hpp>
#include <overlay/OgreShapes.h>
#include "WorldScreenStage.h"


namespace ape {
  namespace visualization {

    WorldScreenStage::WorldScreenStage(AppWindow* appWindow):
        appWindow(appWindow), active(false), overlay() {
      //texture synthesis button
      auto texSynth=std::make_shared<shapes::OgrePolygon2D>(
          appWindow,std::vector<glm::vec2>({
              glm::vec2(+0.85, +0.95),
              glm::vec2(+0.95, +0.95),
              glm::vec2(+0.95, +0.85),
              glm::vec2(+0.85, +0.85)
                                           }),
          Ogre::ColourValue(0.8f,0.0f,0.0f));
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