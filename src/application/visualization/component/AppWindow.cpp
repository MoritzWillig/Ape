//
// Created by moritz on 13.05.17.
//

#include "AppWindow.h"

#include <OGRE/Ogre.h>

//FIXME include font (or make path system independent)
#define FONT_FOLDER "/usr/share/fonts/opentype/freefont/"
#define FONT_FILE_NAME "FreeSans.otf"

namespace ape {
  namespace visualization {

    bool AppWindow::createWindow() {
      // create root
      root = new Ogre::Root();

      // choose renderer
      if (!root->
          showConfigDialog()
          ) {
        return false;
      }
      // initialise root
      root->initialise(false);
      // create main window
      renderWindow = root->createRenderWindow("Main", 320, 240,
                                                            false);
      // create the scene
      sceneMgr = root->createSceneManager(Ogre::ST_GENERIC);
      // add a camera
      mainCam = sceneMgr->createCamera("MainCam");

      // add viewport
      vp = renderWindow->addViewport(mainCam);

      return true;
    }

    void AppWindow::createFont() {
      // get the resource manager
      Ogre::ResourceGroupManager &resGroupMgr = Ogre::ResourceGroupManager::getSingleton();
      // tell it to look at this location
      resGroupMgr.addResourceLocation(FONT_FOLDER, "FileSystem");
      // get the font manager
      Ogre::FontManager &fontMgr = Ogre::FontManager::getSingleton();
      // create a font resource
      Ogre::ResourcePtr font = fontMgr.create("MyFont","General");
      // set as truetype
      font->setParameter("type","truetype");
      // set the .ttf file name
      font->setParameter("source",FONT_FILE_NAME);
      // set the size
      font->setParameter("size","26");
      // set the dpi
      font->setParameter("resolution","96");
      // load the ttf
      font->load();
    }

    void AppWindow::createPanel() {
      // get the overlay manager
      Ogre::OverlayManager& overlayMgr = Ogre::OverlayManager::getSingleton();

      // Create a panel
      Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(
          overlayMgr.createOverlayElement("Panel", "PanelName"));
      panel->setMetricsMode(Ogre::GMM_PIXELS);
      panel->setPosition(10, 10);
      panel->setDimensions(300, 120);

      // Create a text area
      Ogre::TextAreaOverlayElement* textArea = static_cast<Ogre::TextAreaOverlayElement*>(
          overlayMgr.createOverlayElement("TextArea", "TextAreaName"));
      textArea->setMetricsMode(Ogre::GMM_PIXELS);
      textArea->setPosition(0, 0);
      textArea->setDimensions(300, 120);
      textArea->setCharHeight(26);
      // set the font name to the font resource that you just created.
      textArea->setFontName("MyFont");
      // say something
      textArea->setCaption("Hello, World!");

      // Create an overlay, and add the panel
      Ogre::Overlay* overlay = overlayMgr.create("OverlayName");
      overlay->add2D(panel);

      // Add the text area to the panel
      panel->addChild(textArea);

      // Show the overlay
      overlay->show();
    }

    AppWindow::AppWindow() {
      createWindow();
      createFont();
      createPanel();
    }

    void AppWindow::startRendering() {
      /*
      // setup frame listener
      ExampleFrameListener *frameListener= new ExampleFrameListener(renderWindow, mainCam);
      frameListener->showDebugOverlay(false);
      root->addFrameListener(frameListener);
      // start rendering
      root->startRendering();
      */
    }

  }
}