#pragma once
//
// Created by moritz on 05.06.17.
//


#include <cmath>
#include <overlay/OgreButton.h>
#include <OGRE/OgrePolygon.h>
#include <AppWindow.h>

#include "Stage.h"
#include "../visualization.h"
#include "SurfaceSelectionStage.h"

namespace ape {
  namespace visualization {

    class WorldScreenStage: public Stage {
    private:
      glm::vec2 lastMousePosition;

      shapes::Container overlay;
      std::shared_ptr<shapes::OgreButton> texSynthButton;
      std::shared_ptr<shapes::OgreButton> closeButton;

      CustomValueCallback<
          IVisualizationController::OverlayChangeRequestHandler,
          void*>& overlayChangeRequestHandler;

      SurfaceSelectionStage& surfaceSelectionStage;

      std::vector<std::string>* surfaceNames;
    protected:
    public:
      //Default
      WorldScreenStage() = delete;

      WorldScreenStage(
          AppWindow* appWindow,
          CustomValueCallback<
              IVisualizationController::OverlayChangeRequestHandler,
              void*>& overlayChangeRequestHandler,
          SurfaceSelectionStage& surfaceSelectionStage,
          std::vector<std::string>* surfaceNames);

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

      bool isActive();

      void setActive(bool active);

      void update(float delta);

      virtual void processKeyEvent(
          int key, int scancode, int action, int mods) override;

      void processMouseButtonEvent(int button,int action, int mods, bool& cancel) override;

      void processMousePositionEvent(double x, double y) override;
    };

  }
}

