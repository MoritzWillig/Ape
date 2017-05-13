#pragma once
//
// Created by moritz on 13.05.17.
//

#include <OGRE/Ogre.h>
#include <OGRE/OgreTextAreaOverlayElement.h>
#include <OGRE/OgreFontManager.h>

namespace ape {
  namespace visualization {

    class AppWindow {
    private:
      bool createWindow();
      void createFont();
      void createPanel();

      Ogre::Root* root;
      Ogre::RenderWindow* renderWindow;
      Ogre::SceneManager* sceneMgr;
      Ogre::Camera* mainCam;
      Ogre::Viewport* vp;
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
      virtual ~AppWindow() = default;

      void startRendering();
    };

  }
}

