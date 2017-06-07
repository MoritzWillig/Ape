
#include "Shapes.h"

namespace ape {
  namespace visualization {
    namespace shapes {

      Polygon2D::Polygon2D(int count, glm::vec2* shape) :
          poly() {
        for (auto i = 0; i < count; i++) {
          poly.insertVertex(Ogre::Vector3(shape.x, shape.y, 0.0));
        }
      }

      void Polygon2D::setVisible(bool visible) {
        //FIXME
      }


    }
  }
}