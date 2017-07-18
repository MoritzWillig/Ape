#pragma once
//
// Created by moritz on 13.05.17.
//

#include <OgreRay.h>
#include <OGRE/Ogre.h>
#include <OGRE/OgreTextAreaOverlayElement.h>
#include <OGRE/OgreFontManager.h>
#include <glm/mat4x4.hpp>
#include <opencv2/core/mat.hpp>

#include "../application/imageProcessing/component/colorSpaces.h"

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
#include <worldState/Surface.h>

namespace ape {
  namespace visualization {

    class AppWindow
    {
    private:
      bool createWindow();
      void createFont();
      void createPanel();

      void createRessources();
	    void createBackgroundTexture();
      void loadIconMaterial(std::string icoName, std::string matName);
      void createCoordinateAxes();
      void createRenderTexture();

	    void updateBackgroundTexture(
          cv::Mat frame, unsigned int width, unsigned int height);
      void computeColorBalancingParameter();
	    void initScene();

      Ogre::Root* root;
      Ogre::RenderWindow* renderWindow;
      GLFWwindow* glfwWindow;
      Ogre::SceneManager* sceneMgr;
      Ogre::Camera* mainCam;
      Ogre::Viewport* vp;
	    Ogre::Rectangle2D* rect;
	    Ogre::TexturePtr backgroundTexture;
      Ogre::TexturePtr rttTexture;
      Ogre::SceneNode* backgroundNode;
      Ogre::ManualObject* coordAxes;
      Ogre::MaterialPtr cubeMat;
      Ogre::RenderTexture* renderTexture;

      //Uniforms for color balancing
      Ogre::Vector3 meanInput;
      Ogre::Vector3 meanTarget;
      Ogre::Vector3 varianceInput;
      Ogre::Vector3 varianceTarget;

      Ogre::TextAreaOverlayElement* textArea;
      Ogre::TextAreaOverlayElement* supplTextArea;

      //Flags
      bool computeColorBalancing;

      glm::vec2 windowSize;

      float mousePosX;
      float mousePosY;
      ape::visualization::OgreRay* queryRay;
      bool hitTestEnabled;

      //FIXME magic numbers
      int textureWidth = 640;
      int textureHeight = 480;
      float coordAxesLength = 0.026;

      Ogre::NameGenerator nameGenerator;

      struct InternalMaterial {
        Ogre::MaterialPtr surfaceMatPtr;
        Ogre::MaterialPtr buttonMatPtr;
        Ogre::TexturePtr texPtr;
      };

      std::map<std::string,InternalMaterial> materials;

      void castViewPortRay(glm::vec2 position);
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

      void setTextInfo(std::string info);

      void processKeyEvent(int key, int scancode, int action, int mods);

      void processMousePositionEvent(double x, double y);

      void processMouseButtonEvent(int button, int action, int mods);

      typedef void (*KeyEventHandler)(void* custom, int key, int scancode, int action, int mods);
      typedef void (*MousePositionEventHandler)(void* custom, double x, double y);
      typedef void (*MouseButtonEventHandler)(void* custom, int button, int action, int mods);

      CustomValueCallback<KeyEventHandler,void*> keyEventHandler;
      CustomValueCallback<MousePositionEventHandler,void*> mousePositionEventHandler;
      CustomValueCallback<MouseButtonEventHandler,void*> mouseButtonEventHandler;

      typedef void (*SubEntitySelectionEventHandler)(void* custom,
                                                     Ogre::Entity* entity,
                                                     int subIndex);
      CustomValueCallback<SubEntitySelectionEventHandler,void*>
          entitySelectionEventHandler;

      void setProjectionMatrix(const glm::mat3x3 projectionMatrix);

      std::string registerTexture(std::string name, cv::Mat texture);

      Ogre::MaterialPtr getSurfaceMaterial(const std::string surface);
      Ogre::MaterialPtr getButtonMaterial(const std::string surface);
      //TODO the hit test should be triggered by a controller
      //and not be appWindow itself in any appState
      void enableHitTest(bool enabled);

      Ogre::Entity* loadModel(std::string modelFile);

      void closeWindow();

      bool getColorBalancing();

      void setColorBalancing(bool enabled);
    };

  }
}

