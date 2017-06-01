
#include <imageProcessing/CameraStream.h>
#include "glm/glm.hpp"

namespace ape {
  namespace visualization {

    class AppWindow;

    class VisualizationController {
    private:
      imageProcessing::CameraStream* stream;
      AppWindow* scene;
    protected:
    public:
      VisualizationController(imageProcessing::CameraStream* pStream);

      VisualizationController() = delete;

      void startDisplay();

      void showMenus();

      void showScene();

	    void update(float timeStep, const glm::mat4& viewMatrix);

      bool getTerminateRequest();
    };
  }
}