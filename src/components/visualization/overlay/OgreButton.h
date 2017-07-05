#pragma once

#include "OgreShapes.h"

namespace ape {
  namespace visualization {
    namespace shapes {

      class OgreButton: public OgrePolygon2D {
      private:
      protected:
      public:
        //Default
        OgreButton() = default;

        OgreButton(
            AppWindow* appWindow,
            float left,
            float top,
            float width,
            float height,
            Ogre::ColourValue color);

        // Copy constructor
        OgreButton(const OgreButton&) = default;

        // Move constructor
        OgreButton(OgreButton&&) = default;

        // Copy assignment operator
        OgreButton& operator=(const OgreButton&)& = default;

        // Move assignment operator
        OgreButton& operator=(OgreButton&&)& = default;

        // Destructor
        virtual ~OgreButton() = default;
      };

    }
  }
}
