#pragma once

#include <cmath>
#include <overlay/Shapes.h>
#include <overlay/OgreButton.h>
#include <OGRE/OgrePolygon.h>
#include <AppWindow.h>
#include <common/signals/Signal.h>

#include "Stage.h"
#include "../visualization.h"
//FIXME check correct import

namespace ape {
  namespace visualization {

    class SurfaceSelectionStage: public Stage {
    private:
      IVisualizationController* visController;

      shapes::Container overlay;
      std::vector<std::shared_ptr<shapes::OgreButton>> buttons;

      ape::worldState::ISurface* selectedItem;
      ape::worldState::ISurface* hoveredItem;

      int scrollPosition;

      CustomValueCallback<
          IVisualizationController::SurfaceSelectionHandler,
          void*>& surfaceSelectionHandler;

      glm::vec2 lastMousePosition;
    protected:
      void setScrollPosition(int position);
    public:
      //Default
      SurfaceSelectionStage() = delete;

      SurfaceSelectionStage(
          IVisualizationController* visController,
          AppWindow* appWindow,
          CustomValueCallback<
              IVisualizationController::SurfaceSelectionHandler,
              void*>& surfaceSelectionHandler);

      // Copy constructor
      SurfaceSelectionStage(const SurfaceSelectionStage&) = default;

      // Move constructor
      SurfaceSelectionStage(SurfaceSelectionStage&&) = default;

      // Copy assignment operator
      SurfaceSelectionStage&
      operator=(const SurfaceSelectionStage&)& = default;

      // Move assignment operator
      SurfaceSelectionStage&
      operator=(SurfaceSelectionStage&&)& = default;

      // Destructor
      virtual ~SurfaceSelectionStage() = default;

      void setActive(bool active);

      void update(float delta);

      void setSelection(ape::worldState::ISurface* surface);

      void updateShapes();

      virtual void processKeyEvent(
          int key, int scancode, int action, int mods) override;
      virtual void processMousePositionEvent(
          double x, double y) override;
      virtual void processMouseButtonEvent(
          int button, int action, int mods, bool& cancel) override;

      std::vector<std::string> surfaces;

      glm::vec2 dictPosition;
      glm::vec2 dictSize;

      glm::vec2 surfaceButtonSize;
      float surfaceButtonPadding;

    };

  }
}

