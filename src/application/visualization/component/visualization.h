
#include "AppWindow.h"

namespace ape {
  namespace visualization {

    class VisualizationController {
    private:
      AppWindow* scene;
    protected:
    public:

      VisualizationController();

      void startDisplay();

      void showMenus();

      void showScene();

      void update(float timeStep);

      bool getTerminateRequest();
    };
  }
}