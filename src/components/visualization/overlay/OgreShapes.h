#pragma once

#include <glm/vec2.hpp>

#include "Shapes.h"

#include <OGRE/OgrePolygon.h>
#include <OGRE/Ogre.h>
#include <AppWindow.h>
#include <common/signals/Signal.h>

namespace ape {
  namespace visualization {
    namespace shapes {

      class OgrePolygon2D : public Shape {
      private:
        void regenerateOgreObject();
      protected:
        AppWindow* appWindow;
        std::vector<glm::vec2> shape;

        Ogre::ManualObject* ogreObject;
      public:
        OgrePolygon2D(AppWindow* appWindow,
                      std::vector<glm::vec2> shape,
                      Ogre::ColourValue colour);

        virtual ~OgrePolygon2D();

        void updateOgreObject();

        virtual void setVisible(bool visible) override;

        virtual Ogre::MovableObject* getOgreObject() override;

        Ogre::ColourValue colour;
        ValueSignal<std::string> textureName;
      };

    }

  }
}