//
// Created by moritz on 13.05.17.
//

#include "AppWindow.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreFontManager.h>
#include <OGRE/OgreTextAreaOverlayElement.h>


//FIXME include font (or make path system independent)
#define FONT_FOLDER "C:/dev/Uni/VRAR/Ape/assets/fonts"
#define MESH_FOLDER "C:/dev/Uni/VRAR/Ape/assets/meshes"
#define FONT_FILE_NAME "FreeSans.otf"
#define MESH_FILE_NAME "athene.mesh"

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

      mainCam->setPosition(Ogre::Vector3(0.0f,0.0f,150.0f));
      mainCam->lookAt(Ogre::Vector3(0.0f,0.0f,0.0f));
      mainCam->setNearClipDistance(0.5f);
      mainCam->setFarClipDistance(200.0f);

      // add viewport
      vp = renderWindow->addViewport(mainCam);
      vp->setBackgroundColour(Ogre::ColourValue(0,0,1.0));


      return true;
    }

	void AppWindow::createRessources() {
		// load the basic resource location(s)
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(FONT_FOLDER, "FileSystem", "GUI");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(MESH_FOLDER, "FileSystem", "General");

		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("GUI");
		createFont();
		//createMesh();
	}

    void AppWindow::createFont() {
		// get the font manager
		Ogre::FontManager &fontMgr = Ogre::FontManager::getSingleton();
		// create a font resource
		Ogre::ResourcePtr font = fontMgr.create("MyFont", "GUI");
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

	void AppWindow::createMesh() {
		// get the font manager
		Ogre::MeshManager &meshMgr = Ogre::MeshManager::getSingleton();
		meshMgr.load(MESH_FILE_NAME, "General");
		// create a mesh resource
		//Ogre::ResourcePtr mesh = meshMgr.create(MESH_FILE_NAME, "General");
		// set as mesh
		//mesh->setParameter("type", "mesh");
		// set the .mesh file name
		//mesh->setParameter("source", MESH_FILE_NAME);
		// load the mesh
		//mesh->load();
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
      textArea->setCaption("Moin Moin und Hallo");

      // Create an overlay, and add the panel
      Ogre::Overlay* overlay = overlayMgr.create("OverlayName");
      overlay->add2D(panel);

      // Add the text area to the panel
      panel->addChild(textArea);

      // Show the overlay
      overlay->show();
    }

	void AppWindow::initScene() {
		sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
		Ogre::Entity* ogreEntity = sceneMgr->createEntity(MESH_FILE_NAME, MESH_FILE_NAME);
		Ogre::SceneNode* ogreNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		ogreNode->setPosition(0, 0, 0);
		ogreNode->attachObject(ogreEntity);
		Ogre::Light* light = sceneMgr->createLight("MainLight");
		light->setPosition(20, 80, 50);
	}

    AppWindow::AppWindow() {
      createWindow();
      createRessources();
      createPanel();
	  initScene();
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