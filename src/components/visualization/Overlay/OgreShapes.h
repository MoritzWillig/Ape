#pragma once

#include <visualization/overlay/Shapes.h>

namespace ape {
  namespace visualization {

    namespace shapes {

      class OgrePolygon2D : public Polygon2D {
      private:
      protected:
        Ogre::Polygon poly;
      public:

        Polygon2D(int count, glm::vec2 shape[]);

        virtual void setVisible(bool visible) override;
      };

    }

  }
}