
#include "OgreButton.h"

namespace ape {
  namespace visualization {
    namespace shapes {

      OgreButton::OgreButton(
          AppWindow* appWindow,
          float left,
          float top,
          float width,
          float height,
          Ogre::ColourValue color
      ):
          OgrePolygon2D(
              appWindow,
              std::vector<glm::vec2>({
                      glm::vec2(left      ,top),
                      glm::vec2(left+width,top),
                      glm::vec2(left+width,top-height),
                      glm::vec2(left      ,top-height)}),
              std::vector<glm::vec2>({
                      glm::vec2(0,0),
                      glm::vec2(1,0),
                      glm::vec2(1,1),
                      glm::vec2(0,1)}),
              color),
      position(left, top), size(width,height) {
      }

      bool OgreButton::hit(glm::vec2 position) {
        return (
            (position.x >= this->position.x) &&
                (position.y >= this->position.y - size.y) &&
                (position.x <= this->position.x + size.x) &&
                (position.y <= this->position.y)
        );
      }

    }
  }
}