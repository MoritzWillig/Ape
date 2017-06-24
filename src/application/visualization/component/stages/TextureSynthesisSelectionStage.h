#pragma once
//
// Created by moritz on 05.06.17.
//


#include <cmath>
#include <overlay/Shapes.h>
#include <OGRE/OgrePolygon.h>
#include <AppWindow.h>

#include "Stage.h"

namespace ape {
  namespace visualization {

    class TextureSynthesisSelectionStage: public Stage {
    private:
      shapes::Container overlay;
    protected:
    public:
      //Default
      TextureSynthesisSelectionStage() = delete;

      TextureSynthesisSelectionStage(AppWindow* appWindow);

      // Copy constructor
      TextureSynthesisSelectionStage(const TextureSynthesisSelectionStage&) = default;

      // Move constructor
      TextureSynthesisSelectionStage(TextureSynthesisSelectionStage&&) = default;

      // Copy assignment operator
      TextureSynthesisSelectionStage&
      operator=(const TextureSynthesisSelectionStage&)& = default;

      // Move assignment operator
      TextureSynthesisSelectionStage&
      operator=(TextureSynthesisSelectionStage&&)& = default;

      // Destructor
      virtual ~TextureSynthesisSelectionStage() = default;

      void setActive(bool active);

      void update(float delta);
    };

  }
}

