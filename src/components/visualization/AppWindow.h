#pragma once
//
// Created by moritz on 13.05.17.
//

#include <OGRE/Ogre.h>
#include <OGRE/OgreTextAreaOverlayElement.h>
#include <OGRE/OgreFontManager.h>
#include <glm/mat4x4.hpp>
#include <opencv2/core/mat.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#else
#define GLFW_EXPOSE_NATIVE_X11
#endif

#include <GLFW/glfw3native.h>

#include <imageProcessing/CameraStream.h>
#include <common/callbacks/CustomValueCallback.h>

namespace ape {
  namespace visualization {

    class AppWindow {
    private:
      bool createWindow();
      void createFont();
      void createPanel();

  	  void createRessources();
	    void createBackgroundTexture();
      void createCoordinateAxes();

	    void updateBackgroundTexture(
          cv::Mat frame, unsigned int width, unsigned int height);

	    void initScene();

      Ogre::Root* root;
      Ogre::RenderWindow* renderWindow;
      GLFWwindow* glfwWindow;
      Ogre::SceneManager* sceneMgr;
      Ogre::Camera* mainCam;
      Ogre::Viewport* vp;
	    Ogre::Rectangle2D* rect;
	    Ogre::TexturePtr backgroundTexture;
      Ogre::ManualObject* coordAxes;

      //FIXME magic numbers
	    int textureWidth = 640;
	    int textureHeight = 480;
      float coordAxesLength = 0.026;

      Ogre::NameGenerator nameGenerator;
    protected:
    public:
      //Default
      AppWindow();

      // Copy constructor
      AppWindow(const AppWindow&) = default;

      // Move constructor
      AppWindow(AppWindow&&) = default;

      // Copy assignment operator
      AppWindow& operator=(const AppWindow&)& = default;

      // Move assignment operator
      AppWindow& operator=(AppWindow&&)& = default;

      // Destructor
      virtual ~AppWindow();

      void update(
          float timeStep, imageProcessing::CameraStream* stream,
          const glm::mat4x4& viewMatrix);

      bool isClosed();

      Ogre::SceneManager* getSceneMgr();

      std::string createName();

      void setWindowHint(std::string hint);

      void processKeyEvent(int key, int scancode, int action, int mods);

      void processMousePositionEvent(double x, double y);

      void processMouseButtonEvent(int button, int action, int mods);

      typedef void (*KeyEventHandler)(void* custom, int key, int scancode, int action, int mods);
      typedef void (*MousePositionEventHandler)(void* custom, double x, double y);
      typedef void (*MouseButtonEventHandler)(void* custom, int button, int action, int mods);

      CustomValueCallback<KeyEventHandler,void*> keyEventHandler;
      CustomValueCallback<MousePositionEventHandler,void*> mousePositionEventHandler;
      CustomValueCallback<MouseButtonEventHandler,void*> mouseButtonEventHandler;

      void setProjectionMatrix(const glm::mat3x3 projectionMatrix);
    };

  }
}

