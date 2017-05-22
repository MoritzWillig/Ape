
namespace ape {
  namespace visualization {

    class AppWindow;

    class VisualizationController {
    private:
      AppWindow* scene;
    protected:
    public:

      VisualizationController();

      void startDisplay();

      void showMenus();

      void showScene();

	  void update(float timeStep, unsigned char* frameData, int width, int height, double* viewMatrix);

      bool getTerminateRequest();
    };
  }
}