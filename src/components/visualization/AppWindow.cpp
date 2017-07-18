//
// Created by moritz on 13.05.17.
//

#include "AppWindow.h"

#include "math_defines.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreFontManager.h>
#include <OGRE/OgreTextAreaOverlayElement.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imageProcessing/CameraStream.h>


//FIXME hardcoded strings entfernen ...
#define FONT_FOLDER "../../../data/assets/fonts"
#define MESH_FOLDER "../../../data/assets/meshes"
#define TEXTURE_FOLDER "../../../data/assets/surfaces"
#define SHADER_FOLDER "../../../data/assets/shader"
#define FONT_FILE_NAME "FreeSans.otf"
#define ICON_FOLDER "../../../data/assets/icons"

enum QueryFlags
{
  WORLD_OBJECT = 1 << 0,
};

class RenderTargetListener : public Ogre::RenderTargetListener {
private:
  Ogre::SceneNode* backgroundNode;
public:
  RenderTargetListener(Ogre::SceneNode* backgroundNode) : backgroundNode(
      backgroundNode) {}

  void preRenderTargetUpdate(const Ogre::RenderTargetEvent& rte) {
    backgroundNode->setVisible(false);
  }

  void postRenderTargetUpdate(const Ogre::RenderTargetEvent& rte) {
    backgroundNode->setVisible(true);
  }
};

namespace ape {
  namespace visualization {

    void glfw_error_callback(int error, const char* description)
    {
      fprintf(stderr, "GLFW Error: %s\n", description);
    }

    void glfw_key_callback(GLFWwindow* window, int key, int scancode,
                           int action, int mods)
    {
      AppWindow* that=(AppWindow*)glfwGetWindowUserPointer(window);

      that->processKeyEvent(key,scancode,action,mods);
    }

    void glfw_cursor_pos_callback(GLFWwindow* window,
                                       double xpos, double ypos)
    {
      AppWindow* that=(AppWindow*)glfwGetWindowUserPointer(window);

      that->processMousePositionEvent(xpos, ypos);
    }

    void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
      AppWindow* that=(AppWindow*)glfwGetWindowUserPointer(window);

      that->processMouseButtonEvent(button, action, mods);
    }

    bool AppWindow::createWindow() {
      glfwSetErrorCallback(glfw_error_callback);
      if (glfwInit()!=GLFW_TRUE)
      {
        throw std::runtime_error("Could not initialize glfw");
      }

      glfwWindowHint(GLFW_DECORATED, 0);
      glfwWindow = glfwCreateWindow(
          (int)windowSize.x,
          (int)windowSize.y, "My Title", NULL, NULL);
      if (!glfwWindow)
      {
        //FIXME this needs to be called because createWindow is called from
        //the constructor ...
        glfwTerminate();
        throw std::runtime_error("Window or OpenGL context creation failed");
      }
      //set our AppWindow as a user pointer to recover
      //this* in the glfw callbacks
      glfwSetWindowUserPointer(glfwWindow,this);

      glfwMakeContextCurrent(glfwWindow);

      root = new Ogre::Root("", "");

      root->loadPlugin(std::string(OGRE_PLUGIN_DIR) + "/RenderSystem_GL");
      root->loadPlugin(
          std::string(OGRE_PLUGIN_DIR) + "/Plugin_OctreeSceneManager");

      // we do this step just to get an iterator that we can use with setRenderSystem. In a future article
      // we actually will iterate the list to display which renderers are available.
      auto renderSystems = root->getAvailableRenderers();
      root->setRenderSystem(renderSystems[0]);
      root->initialise(false);

      // setup main window; hardcode some defaults for the sake of presentation
      Ogre::NameValuePairList opts;
      opts["resolution"] =
          std::to_string((int)windowSize.x)+"x"+
              std::to_string((int)windowSize.y);
      opts["fullscreen"] = "false";
      opts["vsync"] = "true";
#ifdef WIN32
      opts["externalWindowHandle"]=
          Ogre::StringConverter::toString((size_t)glfwGetWin32Window(glfwWindow));
#else
      opts["externalWindowHandle"] =
        Ogre::StringConverter::toString(glfwGetX11Window(glfwWindow));
#endif
      renderWindow = root->createRenderWindow(
          "title", (int)windowSize.x, (int)windowSize.y, false, &opts);
      setWindowHint("");

      glfwSetKeyCallback(glfwWindow, glfw_key_callback);
      glfwSetCursorPosCallback(glfwWindow, glfw_cursor_pos_callback);
      glfwSetMouseButtonCallback(glfwWindow, glfw_mouse_button_callback);

      // create the scene
      sceneMgr = root->createSceneManager(Ogre::ST_GENERIC);
      // add a camera
      mainCam = sceneMgr->createCamera("MainCam");

      mainCam->setPosition(Ogre::Vector3(1.0f, 1.0f, 1.0f));
      mainCam->lookAt(Ogre::Vector3(0.0f, 0.0f, 0.0f));
      mainCam->setNearClipDistance(0.001f);
      mainCam->setFarClipDistance(200.0f);

      // add viewport
      vp = renderWindow->addViewport(mainCam);
      vp->setBackgroundColour(Ogre::ColourValue(0.2, 0.3, 0.7));

      // Init sceneQuery
      queryRay = new ape::visualization::OgreRay(sceneMgr);

      return true;
    }

    void AppWindow::createRessources() {
      // load the basic resource location(s)
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          FONT_FOLDER, "FileSystem", "GUI");
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          MESH_FOLDER, "FileSystem", "General");
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          TEXTURE_FOLDER, "FileSystem", "General");
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          ICON_FOLDER, "FileSystem", "General");
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        SHADER_FOLDER, "FileSystem", "General");

      Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
      Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("GUI");


      createFont();
      createBackgroundTexture();
      createCoordinateAxes();
    }

    void AppWindow::createFont() {
      // get the resource manager
      Ogre::ResourceGroupManager& resGroupMgr = Ogre::ResourceGroupManager::getSingleton();
      // tell it to look at this location
      resGroupMgr.addResourceLocation(FONT_FOLDER, "FileSystem");
      // get the font manager
      Ogre::FontManager& fontMgr = Ogre::FontManager::getSingleton();
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

    void AppWindow::createBackgroundTexture() {
      backgroundTexture = Ogre::TextureManager::getSingleton().createManual(
          "BackgroundTexture", // name
          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
          Ogre::TEX_TYPE_2D,      // type
          textureWidth, textureHeight,         // width & height
          0,                // number of mipmaps
          Ogre::PF_BYTE_RGB,     // pixel format //FIXME use camera format
          Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE // usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for textures updated very often (e.g. each frame)
      );

      loadIconMaterial("close_en.png","close_ico");
      loadIconMaterial("mv_button.png","texSelect_ico");
      loadIconMaterial("selMat.png","texSelect_material");


      Ogre::MaterialPtr ovMaterial = Ogre::MaterialManager::getSingleton().create(
          "OverlayButton", "General");
      ovMaterial->setDiffuse(1.0, 1.0, 1.0, 1.0);
      ovMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
      ovMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
      ovMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
      ovMaterial->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CullingMode::CULL_NONE);

      Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(
          "Background", "General");
      material->getTechnique(0)->getPass(0)->createTextureUnitState(
          "BackgroundTexture");
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
    }

    void AppWindow::loadIconMaterial(std::string icoName, std::string matName) {
      //load icon set
      Ogre::TexturePtr mt =
          Ogre::TextureManager::getSingleton().load(icoName, "General");
      Ogre::MaterialPtr mM = Ogre::MaterialManager::getSingleton().create(
          matName, "General");
      mM->getTechnique(0)->getPass(0)->createTextureUnitState(mt->getName());

      mM->getTechnique(0)->getPass(0)->setSceneBlending(
          Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);
      mM->setDepthWriteEnabled(false);
      mM->setLightingEnabled(false);
      mM->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CullingMode::CULL_NONE);
    }

    void AppWindow::createCoordinateAxes() {
      coordAxes = sceneMgr->createManualObject("MyAxis");
      coordAxes->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
      coordAxes->position(0, 0, 0);
      coordAxes->colour(1, 0, 0);
      coordAxes->position(coordAxesLength, 0, 0);
      coordAxes->position(0, 0, 0);
      coordAxes->colour(0, 1, 0);
      coordAxes->position(0, coordAxesLength, 0);
      coordAxes->position(0, 0, 0);
      coordAxes->colour(0, 0, 1);
      coordAxes->position(0, 0, coordAxesLength);
      coordAxes->end();
    }

    void AppWindow::createPanel() {
      // get the overlay manager
      Ogre::OverlayManager& overlayMgr = Ogre::OverlayManager::getSingleton();

      Ogre::Overlay* GUIoverlay=overlayMgr.create("GUI");
      GUIoverlay->show();

      // Create a panel
      Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(
          overlayMgr.createOverlayElement("Panel", "PanelName"));
      panel->setMetricsMode(Ogre::GMM_PIXELS);
      panel->setPosition(10, 10);
      panel->setDimensions(300, 120);

      // Create a text area
      textArea = static_cast<Ogre::TextAreaOverlayElement*>(
          overlayMgr.createOverlayElement("TextArea", "TextAreaName"));
      textArea->setMetricsMode(Ogre::GMM_PIXELS);
      textArea->setPosition(100, 0);
      textArea->setDimensions(300, 120);
      textArea->setCharHeight(26);
      // set the font name to the font resource that you just created.
      textArea->setFontName("MyFont");
      // say something
      textArea->setCaption("Color balancing disabled");

      supplTextArea = static_cast<Ogre::TextAreaOverlayElement*>(
          overlayMgr.createOverlayElement("TextArea", "TextAreaName2"));
      supplTextArea->setMetricsMode(Ogre::GMM_PIXELS);
      supplTextArea->setPosition(100, 30);
      supplTextArea->setDimensions(300, 120);
      supplTextArea->setCharHeight(26);
      // set the font name to the font resource that you just created.
      supplTextArea->setFontName("MyFont");
      // say something
      supplTextArea->setCaption("");

      // Create an overlay, and add the panel
      Ogre::Overlay* overlay = overlayMgr.create("OverlayName");
      overlay->add2D(panel);

      // Add the text area to the panel
      panel->addChild(textArea);
      panel->addChild(supplTextArea);

      // Show the overlay
      overlay->show();
    }

    void AppWindow::initScene() {
      // Add Cube
      sceneMgr->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));

#ifdef DEBUG_BUILD
      Ogre::SceneNode* debugNode = sceneMgr->getRootSceneNode()->
          createChildSceneNode("debug");
      debugNode->setPosition(0, 0, 0);
      debugNode->setScale(20.0, 20.0, 20.0);
      debugNode->attachObject(coordAxes);
#endif
      cv::Mat white(cv::Mat(16, 16, CV_8UC3));
      white = cv::Scalar(255, 255, 255);
      registerTexture("BaseWhite", white);

      // Set Lighting
      Ogre::Light* light = sceneMgr->createLight("MainLight");
      light->setType(Ogre::Light::LT_DIRECTIONAL);
      light->setDirection(2, -8, 5);
      light->setPosition(2, -8, 5);

      // Attach background to the scene
      backgroundNode = sceneMgr->getRootSceneNode()->createChildSceneNode(
          "Background");
      backgroundNode->attachObject(rect);
    }

    void AppWindow::createRenderTexture() {
      rttTexture =
        Ogre::TextureManager::getSingleton().createManual(
          "RttTex",
          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
          Ogre::TEX_TYPE_2D,
          renderWindow->getWidth(), renderWindow->getHeight(),
          0,
          Ogre::PF_BYTE_RGB,
          Ogre::TU_RENDERTARGET);

      renderTexture = rttTexture->getBuffer()->getRenderTarget();

      renderTexture->addViewport(mainCam);
      renderTexture->getViewport(0)->setClearEveryFrame(true);
      renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue(0.0, 1.0, 0));
      renderTexture->getViewport(0)->setOverlaysEnabled(false);
      renderTexture->addListener(new RenderTargetListener(backgroundNode));
    }

    AppWindow::AppWindow() :
          root(nullptr), renderWindow(nullptr), glfwWindow(nullptr),
          sceneMgr(nullptr), mainCam(nullptr), vp(nullptr), rect(nullptr),
          backgroundTexture(nullptr), coordAxes(nullptr),
          mousePosX(-1), mousePosY(-1), queryRay(), hitTestEnabled(false),
          nameGenerator("ape"), //fixme magic string
          materials(),
          keyEventHandler(nullptr, nullptr),
          mousePositionEventHandler(nullptr, nullptr),
          mouseButtonEventHandler(nullptr, nullptr),
          entitySelectionEventHandler(nullptr, nullptr),
          computeColorBalancing(false), windowSize(1024.0f, 768.0f)
        {
      createWindow();
      createRessources();
      createPanel();
      initScene();
      createRenderTexture();
    }

    AppWindow::~AppWindow() {
      std::cout << "Destroy App Window" << std::endl;

      renderWindow->removeViewport(vp->getZOrder());
      sceneMgr->destroyCamera(mainCam);
      root->destroySceneManager(sceneMgr);
      renderWindow->destroy();
      delete rect;
      delete root;

      glfwDestroyWindow(glfwWindow);
      glfwTerminate();
    }

    int frameCounter = 0;

    Ogre::Vector3 computeMean(Ogre::TexturePtr texture)
    {
      Ogre::Vector3 meanVec(0,0,0);
      size_t width = texture->getWidth();
      size_t height = texture->getHeight();
      Ogre::Image::Box imageBox;
      Ogre::HardwarePixelBufferSharedPtr returnBuffer = texture->getBuffer();
      const Ogre::PixelBox& returnBufferPixelBox = returnBuffer->lock(imageBox, Ogre::HardwareBuffer::HBL_NORMAL);
      Ogre::uint8 * pixels = static_cast<Ogre::uint8*>(returnBufferPixelBox.data);

      size_t count = 0;
      for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
          std::size_t index = 4*(width*j + i);

          if (pixels[index] == 0 && pixels[index + 1] == 255
            && pixels[index + 2] == 0) {
            continue;
          }

          ape::imageProcessing::colorSpaces::RGB rgb(
            (float)pixels[index] / 255.0f,
            (float)pixels[index + 1] / 255.0f,
            (float)pixels[index + 2] / 255.0f);
          ape::imageProcessing::colorSpaces::LAB lab = rgb.toLAB();

          meanVec[0] += lab.l;
          meanVec[1] += lab.a;
          meanVec[2] += lab.b;
          count++;
        }
      }
      returnBuffer->unlock();
      meanVec[0] = meanVec[0] / count;
      meanVec[1] = meanVec[1] / count;
      meanVec[2] = meanVec[2] / count;
      return meanVec;
    }

    Ogre::Vector3 computeVariance(Ogre::TexturePtr texture, Ogre::Vector3 mean)
    {
      Ogre::Vector3 variance(0, 0, 0);
      size_t width = texture->getWidth();
      size_t height = texture->getHeight();
      Ogre::Image::Box imageBox;
      Ogre::HardwarePixelBufferSharedPtr returnBuffer = texture->getBuffer();
      const Ogre::PixelBox& returnBufferPixelBox = returnBuffer->lock(imageBox, Ogre::HardwareBuffer::HBL_NORMAL);
      Ogre::uint8 * pixels = static_cast<Ogre::uint8*>(returnBufferPixelBox.data);

      size_t count = 0;
      for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            std::size_t index = 4 * (width*j + i);
            if (pixels[index] == 0 && pixels[index + 1] == 255
              && pixels[index + 2] == 0) {
              continue;
            }

            ape::imageProcessing::colorSpaces::RGB rgb(
              (float)pixels[index] / 255.0f,
              (float)pixels[index + 1] / 255.0f,
              (float)pixels[index + 2] / 255.0f);
            ape::imageProcessing::colorSpaces::LAB lab = rgb.toLAB();

            variance[0] += POW2((lab.l - mean[0]));
            variance[1] += POW2((lab.a - mean[1]));
            variance[2] += POW2((lab.b - mean[2]));
            count++;
        }
      }
      returnBuffer->unlock();
      variance[0] = std::sqrt(variance[0] / count);
      variance[1] = std::sqrt(variance[1] / count);
      variance[2] = std::sqrt(variance[2] / count);
      return variance;
    }

    void AppWindow::computeColorBalancingParameter() {
      if (!computeColorBalancing) {
        for (auto mapEntry : materials) {
          Ogre::MaterialPtr textureMaterial = mapEntry.second.surfaceMatPtr;
          Ogre::Technique* technique = textureMaterial->getTechnique(0);
          Ogre::Pass* pass = technique->getPass(0);
          Ogre::GpuProgramParametersSharedPtr params = pass->getFragmentProgramParameters();

          params->setNamedConstant("meanInput", Ogre::Vector3(0, 0, 0));
          params->setNamedConstant("meanTarget", Ogre::Vector3(0, 0, 0));
          params->setNamedConstant("quotient", Ogre::Vector3(1, 1, 1));
        }
      }
      else if (frameCounter % 2 == 0) {
        frameCounter = 0;

        for (auto mapEntry : materials) {
          Ogre::MaterialPtr textureMaterial = mapEntry.second.surfaceMatPtr;
          Ogre::Technique* technique = textureMaterial->getTechnique(0);
          Ogre::Pass* pass = technique->getPass(0);
          Ogre::GpuProgramParametersSharedPtr params = pass->getFragmentProgramParameters();

          params->setNamedConstant("meanInput", Ogre::Vector3(0, 0, 0));
          params->setNamedConstant("meanTarget", Ogre::Vector3(0, 0, 0));
          params->setNamedConstant("quotient", Ogre::Vector3(1, 1, 1));
        }

        renderTexture->update();
        meanInput = computeMean(rttTexture);
        meanTarget = computeMean(backgroundTexture);
        varianceInput = computeVariance(rttTexture, meanInput);
        varianceTarget = computeVariance(backgroundTexture, meanTarget);

        for (auto mapEntry : materials) {
          Ogre::MaterialPtr textureMaterial = mapEntry.second.surfaceMatPtr;
          Ogre::Technique* technique = textureMaterial->getTechnique(0);
          Ogre::Pass* pass = technique->getPass(0);
          Ogre::GpuProgramParametersSharedPtr params = pass->getFragmentProgramParameters();

          params->setNamedConstant("meanInput", meanInput);
          params->setNamedConstant("meanTarget", meanTarget);
          params->setNamedConstant("quotient", varianceInput / varianceTarget);
        }
      }
      frameCounter++;
    }


    void AppWindow::updateBackgroundTexture(
        cv::Mat frame, unsigned int width, unsigned int height) {
      if (textureWidth != width || textureHeight != height) {
        textureWidth = width;
        textureHeight = height;
        createBackgroundTexture();
      }
      // Get the pixel buffer
      Ogre::HardwarePixelBufferSharedPtr pixelBuffer = backgroundTexture->getBuffer();

      // Lock the pixel buffer and get a pixel box
      pixelBuffer->lock(
          Ogre::HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
      const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

      // FIXME check functionality of above and remove this
      unsigned char* pDest = static_cast<unsigned char*>(pixelBox.data);
      unsigned char* frameData=frame.data;

      // Fill in some pixel data. This will give a semi-transparent blue,
      // but this is of course dependent on the chosen pixel format.
      for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {

          *pDest++ = frameData[3 * (width * j + i) + 0]; // B
          *pDest++ = frameData[3 * (width * j + i) + 1]; // G
          *pDest++ = frameData[3 * (width * j + i) + 2]; // R
          *pDest++ = 127; //A
        }

        pDest += pixelBox.getRowSkip() *
                 Ogre::PixelUtil::getNumElemBytes(pixelBox.format);

      }
      pixelBuffer->unlock();
    }

    void AppWindow::update(float timeStep, imageProcessing::CameraStream* stream,
      const glm::mat4x4& viewMatrix)
    {
      glfwPollEvents();

      Ogre::Matrix4 matrix = Ogre::Matrix4(
        viewMatrix[0][0], viewMatrix[0][1], viewMatrix[0][2], viewMatrix[0][3],
        viewMatrix[1][0], viewMatrix[1][1], viewMatrix[1][2], viewMatrix[1][3],
        viewMatrix[2][0], viewMatrix[2][1], viewMatrix[2][2], viewMatrix[2][3],
        viewMatrix[3][0], viewMatrix[3][1], viewMatrix[3][2], viewMatrix[3][3]);
      mainCam->setCustomViewMatrix(true, matrix);

      updateBackgroundTexture(
          stream->getCurrentFrame(),
          stream->getFrameWidth(), stream->getFrameHeight());
      computeColorBalancingParameter();
      root->renderOneFrame(timeStep);
      renderWindow->update(true);

      Ogre::WindowEventUtilities::messagePump();
    }

    bool AppWindow::isClosed() {
      return (glfwWindowShouldClose(glfwWindow)==GLFW_TRUE);
    }

    Ogre::SceneManager* AppWindow::getSceneMgr() {
      return sceneMgr;
    }

    std::string AppWindow::createName() {
      return nameGenerator.generate();
    }

    void AppWindow::setWindowHint(std::string hint) {
      auto title = std::string("Ape!")+
                   std::string((hint=="")?"":" - ")+
                   hint;
      glfwSetWindowTitle(glfwWindow,title.c_str());
    }

    void AppWindow::setTextInfo(std::string info) {
      supplTextArea->setCaption(info);
    }

    void AppWindow::processKeyEvent(int key, int scancode, int action,
      int mods)
    {
      keyEventHandler.callExceptIfNotSet(key,scancode,action,mods);
    }

    void AppWindow::processMousePositionEvent(double x, double y) {
      mousePosX = x / windowSize.x;
      mousePosY = y / windowSize.y;
      mousePositionEventHandler.callExceptIfNotSet(
          2.0 * (mousePosX - 0.5),
          2.0 * (0.5 - mousePosY));
    }

    void AppWindow::processMouseButtonEvent(int button, int action, int mods) {
      mouseButtonEventHandler.callExceptIfNotSet(button, action, mods);
      if ((hitTestEnabled) && (action==GLFW_RELEASE) && (button==GLFW_MOUSE_BUTTON_LEFT)) {
        castViewPortRay(glm::vec2(mousePosX, mousePosY));
      }
    }

    void AppWindow::setProjectionMatrix(const glm::mat3x3 projectionMatrix) {
      // Set CameraProjection Matrix based on calibration parameters
      float znear = 0.001f;
      float zfar = 200.0f;
      int camWidth = 640;
      int camHeight = 480;

      double fx = projectionMatrix[0][0];
      double fy = projectionMatrix[1][1];
      double u0 = projectionMatrix[0][2];
      double v0 = projectionMatrix[1][2];
      double skew = 0.0;

      Ogre::Matrix4 Projection = Ogre::Matrix4(
          (Ogre::Real)(2.0*fx / camWidth), 0, (Ogre::Real)(2.0*(u0 / camWidth) - 1.0), 0,
          0, (Ogre::Real)(2.0*fy / camHeight), (Ogre::Real)(2.0*(v0 / camHeight) - 1.0), 0,
          0, 0, (Ogre::Real)(-1.0*(zfar+znear) / (zfar-znear)), (Ogre::Real)(-2.0*zfar*znear / (zfar-znear)),
          0, 0, -1.0, 0);
      mainCam->setCustomProjectionMatrix(true, Projection);
    }

    std::string
    AppWindow::registerTexture(std::string name, cv::Mat texture) {
      auto textureName = nameGenerator.generate();
      auto surfaceMaterialName= textureName+"_surface_material";
      auto buttonMaterialName = textureName + "_button_material";
      Ogre::TexturePtr ogreTexture = Ogre::TextureManager::getSingleton().createManual(
          textureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
          Ogre::TEX_TYPE_2D, texture.cols, texture.rows, 0,
          Ogre::PF_BYTE_RGB, Ogre::TU_DEFAULT);

      auto buffer=ogreTexture->getBuffer();
      buffer->lock(Ogre::HardwareBuffer::HBL_NORMAL);
      auto pBox=buffer->getCurrentLock();
      auto data= static_cast<unsigned char*>(pBox.data);
      auto tData=texture.data;
      for (auto y=0; y<texture.rows; y++) {
        for (auto x=0; x<texture.cols; x++) {
          *data++=*tData++;
          *data++=*tData++;
          *data++=*tData++;
          *data++=127;
        }

        data+=pBox.getRowSkip()*Ogre::PixelUtil::getNumElemBytes(pBox.format);
      }
      buffer->unlock();

      Ogre::MaterialPtr baseMaterial = Ogre::MaterialManager::getSingleton().getByName("TextureColorBalance");
      Ogre::MaterialPtr material = baseMaterial->clone(surfaceMaterialName);
      material->getTechnique(0)->getPass(0)->getTextureUnitState("DiffuseMap")->setTextureName(textureName);
      material->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CullingMode::CULL_NONE);

      Ogre::MaterialPtr buttonMaterial = Ogre::MaterialManager::getSingleton().create(buttonMaterialName, "General");
      buttonMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(textureName);
      buttonMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
      buttonMaterial->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CullingMode::CULL_NONE);

      InternalMaterial iMat;
      iMat.buttonMatPtr = buttonMaterial;
      iMat.surfaceMatPtr = material;
      iMat.texPtr=ogreTexture;
      materials[name]=iMat;

      return surfaceMaterialName;
    }

    Ogre::MaterialPtr AppWindow::getSurfaceMaterial(const std::string surface) {
      return materials[surface].surfaceMatPtr;
    }

    Ogre::MaterialPtr AppWindow::getButtonMaterial(const std::string surface) {
      return materials[surface].buttonMatPtr;
    }

    void AppWindow::castViewPortRay(glm::vec2 position) {
      Ogre::Ray mouseRay =
          mainCam->getCameraToViewportRay(position.x,position.y);

      Ogre::Vector3 resultVec(0.0f);
      Ogre::MovableObject* resultObj;
      size_t subIndex;

      bool found = queryRay->Raycast(
          mouseRay, WORLD_OBJECT, resultVec, &resultObj, subIndex);

      if (found) {
        Ogre::Entity* entity = static_cast<Ogre::Entity*>(
          resultObj->getParentSceneNode()->getAttachedObject(0));
        entitySelectionEventHandler.callExceptIfNotSet(entity,subIndex);
      } else {
        entitySelectionEventHandler.callExceptIfNotSet(nullptr,-1);
      }
    }

    void AppWindow::enableHitTest(bool enabled) {
      hitTestEnabled=enabled;
    }

    Ogre::Entity* AppWindow::loadModel(std::string modelFile) {
      Ogre::Entity* worldEntity = sceneMgr->createEntity("world",modelFile);
      worldEntity->addQueryFlags(WORLD_OBJECT);
      worldEntity->setMaterial(materials["BaseWhite"].surfaceMatPtr);

      //FIXME do not create & attach to node here
      Ogre::SceneNode* ogreNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
      //FIXME we use this only for loading the world, so we can for now
      // hard-code the transformation...
      ogreNode->rotate(
          Ogre::Vector3(1.0, 0.0, 0.0),
          Ogre::Radian(1.5707963268),
          Ogre::Node::TS_LOCAL);


//#define APETOWN_1
#define APETOWN_2
#ifdef APETOWN_1
      double const f = 1.5;
      ogreNode->setPosition(0.07, 0.00, 0.00);
      ogreNode->setScale(0.007 * f, 0.007 * f, 0.007 * f);
      ogreNode->attachObject(worldEntity);
#endif
#ifdef APETOWN_2
      double const f = 1.0f;
      ogreNode->setPosition(0.04, 0.04, 0.00);
      ogreNode->setScale(0.005 * f, 0.004 * f, 0.0004 * f);
      ogreNode->attachObject(worldEntity);
#endif
      return worldEntity;
    }

    void AppWindow::closeWindow() {
      glfwSetWindowShouldClose(glfwWindow,GLFW_TRUE);
    }

    void AppWindow::setColorBalancing(bool enabled) {
      computeColorBalancing=enabled;

      textArea->setCaption(
          std::string("Color balancing ")+
              (computeColorBalancing?"enabled":"disabled"));
    }

    bool AppWindow::getColorBalancing() {
      return computeColorBalancing;
    }

  }
}
