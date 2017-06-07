#pragma once

#include <vector>
#include <memory>

namespace ape {
  namespace visualization {

    namespace shapes {

      class Shape {
      private:
      protected:
      public:
        virtual void setVisible(bool visible) = 0;

        virtual ~Shape() = default;
      };

      class Container : public Shape {
      private:
      protected:
      public:
        std::vector<std::shared_ptr<Shape>> childs;

        virtual void setVisible(bool visible) override;

        virtual ~Container() = default;
      };

    }

  }
}
