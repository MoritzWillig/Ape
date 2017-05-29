//
// Created by moritz on 13.05.17.
//

#include "AppWindow.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreFontManager.h>
#include <OGRE/OgreTextAreaOverlayElement.h>


#define FONT_FOLDER "data/assets/fonts"
#define MESH_FOLDER "data/assets/meshes"
#define TEXTURE_FOLDER "C:/dev/Uni/VRAR/Ape/assets/textures"
#define FONT_FILE_NAME "FreeSans.otf"
#define MESH_FILE_NAME "cube.mesh"

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

	void AppWindow::createRessources() {
		// load the basic resource location(s)
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(FONT_FOLDER, "FileSystem", "GUI");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(MESH_FOLDER, "FileSystem", "General");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(TEXTURE_FOLDER, "FileSystem", "General");

		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("GUI");
		createFont();
		createBackgroundTexture();
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

	void AppWindow::createBackgroundTexture() {
		backgroundTexture = Ogre::TextureManager::getSingleton().createManual(
			"BackgroundTexture", // name
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D,      // type
			textureWidth, textureHeight,         // width & height
			0,                // number of mipmaps
			Ogre::PF_BYTE_BGRA,     // pixel format
			Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE // usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for textures updated very often (e.g. each frame)
		);

		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Background", "General");
		material->getTechnique(0)->getPass(0)->createTextureUnitState("BackgroundTexture");
		material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
		material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
		material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

		// Create background rectangle covering the whole screen
		rect = new Ogre::Rectangle2D(true);
		rect->setCorners(-1.0, 1.0, 1.0, -1.0);
		rect->setMaterial("Background");

		// Render the background before everything else
		rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

		// Use infinite AAB to always stay visible
		Ogre::AxisAlignedBox aabInf;
		aabInf.setInfinite();
		rect->setBoundingBox(aabInf);



		// Example of background scrolling
		//material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.25, 0.0);
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

	void AppWindow::initScene() {
		// Add Cube
		sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
		Ogre::Entity* ogreEntity = sceneMgr->createEntity(MESH_FILE_NAME, MESH_FILE_NAME);
		Ogre::SceneNode* ogreNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		ogreNode->setPosition(0, 0, 0);
		ogreNode->setScale(0.001, 0.001, 0.001);
		ogreNode->attachObject(ogreEntity);

		// Set Lighting
		Ogre::Light* light = sceneMgr->createLight("MainLight");
		light->setPosition(-20, 80, -50);

		// Attach background to the scene
		Ogre::SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode("Background");
		node->attachObject(rect);
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
	  delete rect;
      delete root;
    }

	void AppWindow::updateBackgroundTexture(unsigned char* frameData, int width, int height) {
		if (textureWidth != width || textureHeight != height) {
			textureWidth = width;
			textureHeight = height;
			createBackgroundTexture();
		}
		// Get the pixel buffer
		Ogre::HardwarePixelBufferSharedPtr pixelBuffer = backgroundTexture->getBuffer();

		// Lock the pixel buffer and get a pixel box
		pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
		const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

		unsigned char* pDest = static_cast<unsigned char*>(pixelBox.data);


		// Fill in some pixel data. This will give a semi-transparent blue,
		// but this is of course dependent on the chosen pixel format.
		for (size_t j = 0; j < height; j++)
		{
			for (size_t i = 0; i < width; i++)
			{
				*pDest++ = frameData[3 * (width*j + i) + 0]; // B
				*pDest++ = frameData[3 * (width*j + i) + 1]; // G
				*pDest++ = frameData[3 * (width*j + i) + 2]; // R
				*pDest++ = 127; // A
			}

			pDest += pixelBox.getRowSkip() * Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
		}

		// Unlock the pixel buffer
		pixelBuffer->unlock();
	}
    void AppWindow::update(
				float timeStep, unsigned char* frameData, int width, int height,
        const glm::mat4& viewMatrix) {
		//FrameListener listener(renderWindow); // Add the simple frame listener.
		//root->addFrameListener(&listener);
		//root->startRendering(); //we implement our own loop


		Ogre::Matrix4 matrix;
      //FIXME the other constructor is only available in newer OGRE
      //FIXME update OGRE
/*
      Ogre::Matrix4 matrix = Ogre::Matrix4(viewMatrix[0], viewMatrix[1], viewMatrix[2], viewMatrix[3],
                                           viewMatrix[4], viewMatrix[5], viewMatrix[6], viewMatrix[7],
                                           viewMatrix[8], viewMatrix[9], viewMatrix[10], viewMatrix[11],
                                           viewMatrix[12], viewMatrix[13], viewMatrix[14], viewMatrix[15]);
    */
		mainCam->setCustomViewMatrix(true, matrix);
		//mainCam->setDirection(Ogre::Vector3(rotation[0], rotation[1], rotation[2]));
		//Ogre::Vector3 position = Ogre::Vector3(-translation[0], -translation[1], translation[2]);
		//printf("Position: (%f, %f, %f)", position.x, position.y, position.z);
		//mainCam->setPosition(position);

		updateBackgroundTexture(frameData, width, height);
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