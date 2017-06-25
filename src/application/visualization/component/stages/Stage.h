#pragma once

#include <AppWindow.h>

namespace ape {
  namespace visualization {

    class Stage {
    private:
    protected:
      AppWindow* appWindow;

      bool active;
    public:
      //Default
      Stage() = delete;

      Stage(AppWindow* appWindow);

      // Copy constructor
      Stage(const Stage&) = default;

      // Move constructor
      Stage(Stage&&) = default;

      // Copy assignment operator
      Stage& operator=(const Stage&)& = default;

      // Move assignment operator
      Stage& operator=(Stage&&)& = default;

      // Destructor
      virtual ~Stage() = default;

      virtual void processKeyEvent(
          int key, int scancode, int action, int mods);
      virtual void processMousePositionEvent(
          double x, double y);
      virtual void processMouseButtonEvent(
          int button, int action, int mods);
    };

  }
}

