
#include "Shapes.h"

namespace ape {
  namespace visualization {

    namespace shapes {

      void Container::setVisible(bool visible) {
        for (auto child: childs) {
          child->setVisible(visible);
        }
      }

      Ogre::MovableObject* Container::getOgreObject() {
        return nullptr;
      }

    }
  }
}
