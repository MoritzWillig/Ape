#pragma once

#include <glm/vec2.hpp>

#include "Shapes.h"

#include <OGRE/OgrePolygon.h>
#include <OGRE/Ogre.h>
#include <AppWindow.h>

namespace ape {
  namespace visualization {
    namespace shapes {

      class OgrePolygon2D : public Shape {
      private:
      protected:
        AppWindow* appWindow;
        std::vector<glm::vec2> shape;

        Ogre::ManualObject* ogreObject;

        void updateOgreObject();
      public:
        OgrePolygon2D(AppWindow* appWindow,
                      std::vector<glm::vec2> shape,
                      Ogre::ColourValue colour);

        virtual void setVisible(bool visible) override;

        virtual Ogre::MovableObject* getOgreObject() override;

        Ogre::ColourValue colour;
      };

    }

  }
}