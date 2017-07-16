#pragma once

#include <vector>
#include <memory>
#include <OGRE/OgreMovableObject.h>

namespace ape {
  namespace visualization {

    namespace shapes {

      class Shape {
      private:
        bool visible;
      protected:
      public:
        Shape();

        virtual ~Shape() = default;

        bool isVisible();

        virtual void setVisible(bool visible);

        virtual Ogre::MovableObject* getOgreObject() = 0;
      };

      class Container : public Shape {
      private:
      protected:
      public:
        std::vector<std::shared_ptr<Shape>> childs;

        virtual ~Container() = default;

        virtual void setVisible(bool visible) override;

        virtual Ogre::MovableObject* getOgreObject() override;
      };

    }

  }
}
