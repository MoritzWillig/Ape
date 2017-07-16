
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

      Shape::Shape():
        visible(true) {
      }

      bool Shape::isVisible() {
        return visible;
      }

      void Shape::setVisible(bool visible) {
        this->visible=visible;

      }
    }
  }
}
