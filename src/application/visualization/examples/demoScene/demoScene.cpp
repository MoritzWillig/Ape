#include <iostream>
#include <chrono>
#include <thread>

#include "../../component/visualization.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreFontManager.h>
#include <OGRE/OgreTextAreaOverlayElement.h>

#include "TerrainGenerator.h"


#define FONT_FOLDER "../../../data/assets/fonts"
#define MESH_FOLDER "../../../data/assets/meshes"
#define TEXTURE_FOLDER "C:/dev/Uni/VRAR/Ape/assets/textures"
#define FONT_FILE_NAME "FreeSans.otf"
#define MESH_FILE_NAME "cube.mesh"

namespace ape {

}

Ogre::Root* root;
Ogre::RenderWindow* renderWindow;
Ogre::SceneManager* sceneMgr;
Ogre::Camera* mainCam;
Ogre::Viewport* vp;

//FIXME magic numbers
int textureWidth = 640;
int textureHeight = 480;



bool createWindow() {
  root = new Ogre::Root("", "");

  root->loadPlugin(std::string(OGRE_PLUGIN_DIR) + "/RenderSystem_GL");
  root->loadPlugin(std::string(OGRE_PLUGIN_DIR) + "/Plugin_OctreeSceneManager");

  // we do this step just to get an iterator that we can use with setRenderSystem. In a future article
  // we actually will iterate the list to display which renderers are available.
  auto renderSystems = root->getAvailableRenderers();
  root->setRenderSystem(renderSystems[0]);
  root->initialise(false);

  // load the basic resource location(s)
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

  mainCam->setPosition(Ogre::Vector3(15.0f, 30.0f, 15.0f));
  mainCam->lookAt(Ogre::Vector3(0.0f, 0.0f, 0.0f));
  mainCam->setNearClipDistance(0.05f);
  mainCam->setFarClipDistance(200.0f);

  // add viewport
  vp = renderWindow->addViewport(mainCam);
  vp->setBackgroundColour(Ogre::ColourValue(0.2, 0.3, 0.7));

  return true;
}

void createFont() {
  // get the resource manager
  Ogre::ResourceGroupManager &resGroupMgr = Ogre::ResourceGroupManager::getSingleton();
  // tell it to look at this location
  resGroupMgr.addResourceLocation(FONT_FOLDER, "FileSystem");
  // get the font manager
  Ogre::FontManager &fontMgr = Ogre::FontManager::getSingleton();
  // create a font resource
  Ogre::ResourcePtr font = fontMgr.create("MyFont", "General");
  // set as truetype
  font->setParameter("type", "truetype");
  // set the .ttf file name
  font->setParameter("source", FONT_FILE_NAME);
  // set the size
  font->setParameter("size", "26");
  // set the dpi
  font->setParameter("resolution", "96");
  // load the ttf
  font->load();
}

void createRessources() {
  // load the basic resource location(s)
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(FONT_FOLDER, "FileSystem", "GUI");
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(MESH_FOLDER, "FileSystem", "General");
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(TEXTURE_FOLDER, "FileSystem", "General");

  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("GUI");
  createFont();
}


void createPanel() {
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
  textArea->setCaption("Hello, ModelBasedWorld!");

  // Create an overlay, and add the panel
  Ogre::Overlay* overlay = overlayMgr.create("OverlayName");
  overlay->add2D(panel);

  // Add the text area to the panel
  panel->addChild(textArea);

  // Show the overlay
  overlay->show();
}

void createManualObject() {
  Ogre::ManualObject* mObject = NULL;
  {
    // The manualObject creation requires a name.
    Ogre::String mObjectName = "CubeWithAxes";
    mObject = sceneMgr->createManualObject(mObjectName);

    // Always tell if you want to update the 3D (vertex/index) later or not.
    bool lDoIWantToUpdateItLater = false;
    mObject->setDynamic(lDoIWantToUpdateItLater);

    // Here I create a cube in a first part with triangles, and then axes (in red/green/blue).

    // BaseWhiteNoLighting is the name of a material that already exist inside Ogre.
    // Ogre::RenderOperation::OT_TRIANGLE_LIST is a kind of primitive.
    float lSize = 0.7f;
    mObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    {
      float cp = 1.0f * lSize;
      float cm = -1.0f * lSize;

      mObject->position(cm, cp, cm);// a vertex
      mObject->colour(Ogre::ColourValue(0.0f, 1.0f, 0.0f, 1.0f));
      mObject->position(cp, cp, cm);// a vertex
      mObject->colour(Ogre::ColourValue(1.0f, 1.0f, 0.0f, 1.0f));
      mObject->position(cp, cm, cm);// a vertex
      mObject->colour(Ogre::ColourValue(1.0f, 0.0f, 0.0f, 1.0f));
      mObject->position(cm, cm, cm);// a vertex
      mObject->colour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));

      mObject->position(cm, cp, cp);// a vertex
      mObject->colour(Ogre::ColourValue(0.0f, 1.0f, 1.0f, 1.0f));
      mObject->position(cp, cp, cp);// a vertex
      mObject->colour(Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f));
      mObject->position(cp, cm, cp);// a vertex
      mObject->colour(Ogre::ColourValue(1.0f, 0.0f, 1.0f, 1.0f));
      mObject->position(cm, cm, cp);// a vertex
      mObject->colour(Ogre::ColourValue(0.0f, 0.0f, 1.0f, 1.0f));

      // face behind / front
      mObject->triangle(0, 1, 2);
      mObject->triangle(2, 3, 0);
      mObject->triangle(4, 6, 5);
      mObject->triangle(6, 4, 7);

      // face top / down
      mObject->triangle(0, 4, 5);
      mObject->triangle(5, 1, 0);
      mObject->triangle(2, 6, 7);
      mObject->triangle(7, 3, 2);

      // face left / right
      mObject->triangle(0, 7, 4);
      mObject->triangle(7, 0, 3);
      mObject->triangle(1, 5, 6);
      mObject->triangle(6, 2, 1);
    }
    mObject->end();
    // Here I have finished my ManualObject construction.
    // It is possible to add more begin()-end() thing, in order to use
    // different rendering operation types, or different materials in 1 ManualObject.
    mObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
    {
      float lAxeSize = 2.0f * lSize;
      mObject->position(0.0f, 0.0f, 0.0f);
      mObject->colour(Ogre::ColourValue::Red);
      mObject->position(lAxeSize, 0.0f, 0.0f);
      mObject->colour(Ogre::ColourValue::Red);
      mObject->position(0.0f, 0.0f, 0.0f);
      mObject->colour(Ogre::ColourValue::Green);
      mObject->position(0.0, lAxeSize, 0.0);
      mObject->colour(Ogre::ColourValue::Green);
      mObject->position(0.0f, 0.0f, 0.0f);
      mObject->colour(Ogre::ColourValue::Blue);
      mObject->position(0.0, 0.0, lAxeSize);
      mObject->colour(Ogre::ColourValue::Blue);

      mObject->index(0);
      mObject->index(1);
      mObject->index(2);
      mObject->index(3);
      mObject->index(4);
      mObject->index(5);
    }
    mObject->end();
  }
  Ogre::String lNameOfTheMesh = "MeshCubeAndAxe";
  Ogre::String lResourceGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
  mObject->convertToMesh(lNameOfTheMesh);

  // Now I can create several entities using that mesh
  int lNumberOfEntities = 5;
  for (int iter = 0; iter < lNumberOfEntities; ++iter)
  {
    Ogre::Entity* lEntity = sceneMgr->createEntity(lNameOfTheMesh);
    // Now I attach it to a scenenode, so that it becomes present in the scene.
    Ogre::SceneNode* lNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    lNode->attachObject(lEntity);
    // I move the SceneNode so that it is visible to the camera.
    float lPositionOffset = float(1 + iter * 2) - (float(lNumberOfEntities));
    lNode->translate(lPositionOffset, lPositionOffset, -10.0f);
  }
}

void createWorldMesh() {

}

void initScene() {
  // Add Cube
  sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  Ogre::Entity* cube = sceneMgr->createEntity(MESH_FILE_NAME, MESH_FILE_NAME);
  Ogre::SceneNode* ogreNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
  ogreNode->setPosition(0, 0, 0);
  ogreNode->setScale(0.1, 0.1, 0.1);
 // ogreNode->attachObject(cube);

  // Set Lighting
  Ogre::Light* light = sceneMgr->createLight("MainLight");
  light->setPosition(0, 50, 0);
  sceneMgr->getRootSceneNode()->attachObject(light);

  createManualObject();
}

int main(int argc, char** argv) {
  std::cout<<"Demo scene"<<std::endl;

  createWindow();
  createRessources();
  createPanel();
  createWorldMesh();
  initScene();


  auto frameTime = 1.0f / 30.0f;

  while (!renderWindow->isClosed()) {

    root->renderOneFrame(frameTime);
    renderWindow->update(true);

    //we need this ...
    Ogre::WindowEventUtilities::messagePump();
  }

  std::cout<<"Terminated"<<std::endl;

  return 0;
}
