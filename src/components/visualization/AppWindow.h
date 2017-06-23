#pragma once
//
// Created by moritz on 13.05.17.
//

#include <OGRE/Ogre.h>
#include <OGRE/OgreTextAreaOverlayElement.h>
#include <OGRE/OgreFontManager.h>
#include <glm/mat4x4.hpp>
#include <imageProcessing/CameraStream.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

namespace ape {
  namespace visualization {

    class AppWindow {
    private:
      bool createWindow();
      void createFont();
      void createPanel();

  	  void createRessources();
	    void createBackgroundTexture();

	    void updateBackgroundTexture(
          unsigned char* frameData, unsigned int width, unsigned int height);

	    void initScene();

      Ogre::Root* root;
      Ogre::RenderWindow* renderWindow;
      GLFWwindow* glfwWindow;
      Ogre::SceneManager* sceneMgr;
      Ogre::Camera* mainCam;
      Ogre::Viewport* vp;
	  Ogre::Rectangle2D* rect;
	  Ogre::TexturePtr backgroundTexture;

      //FIXME magic numbers
	  int textureWidth = 640;
	  int textureHeight = 480;

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
          const glm::mat4& viewMatrix);

      bool isClosed();

      Ogre::SceneManager* getSceneMgr();

      std::string createName();

    };

  }
}

