//
// Created by moritz on 13.05.17.
//

#include "AppWindow.h"

#include <OGRE/Ogre.h>

//FIXME include font (or make path system independent)
#define FONT_FOLDER "/usr/share/fonts/opentype/freefont/"
#define FONT_FILE_NAME "FreeSans.otf"

//FIXME remove
class FrameListener : public Ogre::FrameListener {
private:
  Ogre::RenderWindow* renderWindow;
public:
  FrameListener(Ogre::RenderWindow* renderWindow): renderWindow(renderWindow) {}
  bool frameStarted(const Ogre::FrameEvent& evt) {
    // Stop render if main window is closed.
    return !renderWindow->isClosed();
  }
  bool frameEnded(const Ogre::FrameEvent &evt) { return true; }
};  // End of FrameListener class

namespace ape {
  namespace visualization {

    bool AppWindow::createWindow() {
      root = new Ogre::Root("", "");

      root->loadPlugin(std::string(OGRE_PLUGIN_DIR)+"/RenderSystem_GL");
      root->loadPlugin(std::string(OGRE_PLUGIN_DIR)+"/Plugin_OctreeSceneManager");

      // we do this step just to get an iterator that we can use with setRenderSystem. In a future article
      // we actually will iterate the list to display which renderers are available.
      auto renderSystems = root->getAvailableRenderers();
      root->setRenderSystem(renderSystems[0]);
      root->initialise(false);

      // load the basic resource location(s)
      //Ogre::ResourceGroupManager::getSingleton().addResourceLocation("resource", "FileSystem", "General");
      //Ogre::ResourceGroupManager::getSingleton().addResourceLocation("resource/gui.lcd", "Zip", "GUI");
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(FONT_FOLDER, "FileSystem", "GUI");

      Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
      Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("GUI");

      // setup main window; hardcode some defaults for the sake of presentation
      Ogre::NameValuePairList opts;
      opts["resolution"] = "1024x768";
      opts["fullscreen"] = "false";
      opts["vsync"] = "true";

      renderWindow = root->createRenderWindow("Main", 1024, 768, false, &opts);

      // create the scene
      sceneMgr = root->createSceneManager(Ogre::ST_GENERIC);
      // add a camera
      mainCam = sceneMgr->createCamera("MainCam");

      mainCam->setPosition(Ogre::Vector3(0.0f,0.0f,500.0f));
      mainCam->lookAt(Ogre::Vector3(0.0f,0.0f,0.0f));
      mainCam->setNearClipDistance(0.05f);
      mainCam->setFarClipDistance(200.0f);

      // add viewport
      vp = renderWindow->addViewport(mainCam);
      vp->setBackgroundColour(Ogre::ColourValue(0.2,0.3,0.7));

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

    AppWindow::~AppWindow() {
      renderWindow->removeViewport(vp->getZOrder());
      sceneMgr->destroyCamera(mainCam);
      root->destroySceneManager(sceneMgr);
      renderWindow->destroy();
      delete root;
    }

    void AppWindow::update(float timeStep) {
      //FrameListener listener(renderWindow); // Add the simple frame listener.
      //root->addFrameListener(&listener);
      //root->startRendering(); //we implement our own loop

      root->renderOneFrame(timeStep);
      renderWindow->update(true);

      //we need this ...
      Ogre::WindowEventUtilities::messagePump();
    }

    bool AppWindow::isClosed() {
      return renderWindow->isClosed();
    }

  }
}