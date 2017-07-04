
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
              color) {
      }

    }
  }
}