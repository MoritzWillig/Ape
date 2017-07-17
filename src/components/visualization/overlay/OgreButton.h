#pragma once

#include "OgreShapes.h"

namespace ape {
  namespace visualization {
    namespace shapes {

      class OgreButton: public OgrePolygon2D {
      private:
        glm::vec2 position;
        glm::vec2 size;
      protected:
        void generateShape();
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

        bool hit(glm::vec2 position);

        void setPosition(glm::vec2 position);

        void setSize(glm::vec2 size);

        virtual void updateOgreObject() override;
      };

    }
  }
}
