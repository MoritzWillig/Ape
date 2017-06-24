#pragma once
//
// Created by moritz on 05.06.17.
//


#include <cmath>
#include <overlay/Shapes.h>
#include <OGRE/OgrePolygon.h>
#include <AppWindow.h>

#include "Stage.h"
#include "../visualization.h"

namespace ape {
  namespace visualization {

    class WorldScreenStage: public Stage {
    private:
      shapes::Container overlay;

      CustomValueCallback<
          IVisualizationController::OverlayChangeRequestHandler,
          void*>& overlayChangeRequestHandler;
    protected:
    public:
      //Default
      WorldScreenStage() = delete;

      WorldScreenStage(
          AppWindow* appWindow,
          CustomValueCallback<
              IVisualizationController::OverlayChangeRequestHandler,
              void*>& overlayChangeRequestHandler);

      // Copy constructor
      WorldScreenStage(const WorldScreenStage&) = default;

      // Move constructor
      WorldScreenStage(WorldScreenStage&&) = default;

      // Copy assignment operator
      WorldScreenStage&
      operator=(const WorldScreenStage&)& = default;

      // Move assignment operator
      WorldScreenStage& operator=(WorldScreenStage&&)& = default;

      // Destructor
      virtual ~WorldScreenStage() = default;

      void setActive(bool active);

      void update(float delta);

      virtual void processKeyEvent(
          int key, int scancode, int action, int mods);
    };

  }
}

