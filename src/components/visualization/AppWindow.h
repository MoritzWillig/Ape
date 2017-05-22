#pragma once
//
// Created by moritz on 13.05.17.
//

#include <OGRE/Ogre.h>


namespace ape {
  namespace visualization {

    class AppWindow {
    private:
      bool createWindow();
      void createPanel();

	  void createRessources();
	  void createFont();
	  void createCoordinateAxes();
	  void createBackgroundTexture();

	  void updateBackgroundTexture(unsigned char* frameData, int width, int height);

	  void initScene();

      Ogre::Root* root;
      Ogre::RenderWindow* renderWindow;
      Ogre::SceneManager* sceneMgr;
      Ogre::Camera* mainCam;
      Ogre::Viewport* vp;
	  Ogre::Rectangle2D* rect;
	  Ogre::TexturePtr backgroundTexture;
	  Ogre::ManualObject* coordAxes;
	  int textureWidth = 640;
	  int textureHeight = 480;
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

      void update(float timeStep, unsigned char* frameData, int width, int height, double* viewMatrix);

      bool isClosed();
    };

  }
}

