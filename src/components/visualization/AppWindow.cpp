//
// Created by moritz on 13.05.17.
//

#include "AppWindow.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreFontManager.h>
#include <OGRE/OgreTextAreaOverlayElement.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imageProcessing/CameraStream.h>


#define FONT_FOLDER "../../../data/assets/fonts"
#define MESH_FOLDER "../../../data/assets/meshes"
#define TEXTURE_FOLDER "../../../data/assets/surfaces"
#define SHADER_FOLDER "../../../data/assets/shader"
#define FONT_FILE_NAME "FreeSans.otf"

enum QueryFlags
{
  WORLD_OBJECT = 1 << 0,
};

class RenderTargetListener : public Ogre::RenderTargetListener {
  private:
    Ogre::SceneNode* backgroundNode;
  public:
    RenderTargetListener(Ogre::SceneNode* backgroundNode) : backgroundNode(backgroundNode) {}
  void RenderTargetListener::preRenderTargetUpdate(const Ogre::RenderTargetEvent& rte)
  {
    backgroundNode->setVisible(false);
  }

  void RenderTargetListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& rte)
  {
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

      glfwWindow = glfwCreateWindow(1024, 768, "My Title", NULL, NULL);
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
      opts["resolution"] = "1024x768";
      opts["fullscreen"] = "false";
      opts["vsync"] = "true";
#ifdef WIN32
      opts["externalWindowHandle"]=
          Ogre::StringConverter::toString((size_t)glfwGetWin32Window(glfwWindow));
#else
      opts["externalWindowHandle"] =
        Ogre::StringConverter::toString(glfwGetX11Window(glfwWindow));
#endif
      renderWindow = root->createRenderWindow("title", 1024, 768, false, &opts);
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

    void AppWindow::initScene() {
      // Add Cube
      sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

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
      light->setDirection(-2, 8, -5);
      light->setPosition(-2, 8, -5);

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

/*    void AppWindow::initInputHandler() {
      Ogre::OIS
      OIS::ParamList pl;
      size_t windowHnd = 0;
      std::ostringstream windowHndStr;
      m_win->getCustomAttribute("WINDOW", &windowHnd);
      windowHndStr << windowHnd;
      pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
      m_InputManager = OIS::InputManager::createInputSystem( pl );
    }*/

    AppWindow::AppWindow():
        root(nullptr), renderWindow(nullptr), glfwWindow(nullptr),
        sceneMgr(nullptr), mainCam(nullptr), vp(nullptr), rect(nullptr),
        backgroundTexture(nullptr), coordAxes(nullptr),
        mousePosX(-1), mousePosY(-1), queryRay(),
        nameGenerator("ape"), //fixme magic string
        materials(),
        keyEventHandler(nullptr, nullptr),
        mousePositionEventHandler(nullptr, nullptr),
        mouseButtonEventHandler(nullptr, nullptr),
        entitySelectionEventHandler(nullptr, nullptr)
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

    void BGRtoLAlphaBeta(Ogre::Image image) {
      float oneOverRootTwo = 1 / sqrt(2);
      float oneOverRootThree = 1 / sqrt(3);
      float oneOverRootSix = 1 / sqrt(6);
      Ogre::PixelBox pixelBox = image.getPixelBox();
      Ogre::uint8 * data = static_cast<Ogre::uint8*>(pixelBox.data);
      for (size_t j = 0; j < image.getHeight(); j++) {
        for (size_t i = 0; i < image.getWidth(); i++) {
          std::size_t index = 4 * (image.getWidth()*j + i);

          float B = (float)data[index]; //blue
          float G = (float)data[index + 1]; //green
          float R = (float)data[index + 2]; //red

          float L = 0.3811 * R + 0.5783 * G + 0.0402 * B;
          float M = 0.1967 * R + 1.1834 * G + 0.0782 * B;
          float S = 0.0241 * R + 0.1288 * G + 0.8444 * B;

          L = Ogre::Math::LogN(L, 10);
          M = Ogre::Math::LogN(M, 10);
          S = Ogre::Math::LogN(S, 10);

          float l = oneOverRootThree * L + oneOverRootThree * M + oneOverRootThree * S;
          float alpha = oneOverRootSix * L + oneOverRootSix * M - 2 * oneOverRootSix * S;
          float beta = oneOverRootTwo * L - oneOverRootTwo * M;

          data[index] = l;
          data[index + 1] = alpha;
          data[index + 2] = beta;
        }
      }
    }

    void BGRtoLAlphaBeta(Ogre::ColourValue& color) {
      float oneOverRootTwo = 1 / sqrt(2);
      float oneOverRootThree = 1 / sqrt(3);
      float oneOverRootSix = 1 / sqrt(6);

      float L = 0.3811 * color.r + 0.5783 * color.g + 0.0402 * color.b;
      float M = 0.1967 * color.r + 1.1834 * color.g + 0.0782 * color.b;
      float S = 0.0241 * color.r + 0.1288 * color.g + 0.8444 * color.b;

      // Workaround to fix undefined log of 0
      if (L == 0)
        L = 1 / 255.f;
      if (M == 0)
        M = 1 / 255.f;
      if (S == 0)
        S = 1 / 255.f;

      L = std::log10(L);
      M = std::log10(M);
      S = std::log10(S);

      float l = oneOverRootThree * L + oneOverRootThree * M + oneOverRootThree * S;
      float alpha = oneOverRootSix * L + oneOverRootSix * M - 2 * oneOverRootSix * S;
      float beta = oneOverRootTwo * L - oneOverRootTwo * M;

      color.r = l;
      color.g = alpha;
      color.b = beta;
    }

    int frameCounter = 0;

    Ogre::Vector3 computeMean(Ogre::TexturePtr texture) {
      Ogre::Vector3 meanVec(0,0,0);
      size_t width = texture->getWidth();
      size_t height = texture->getHeight();
      Ogre::Image::Box imageBox;
      Ogre::HardwarePixelBufferSharedPtr returnBuffer = texture->getBuffer();
      const Ogre::PixelBox& returnBufferPixelBox = returnBuffer->lock(imageBox, Ogre::HardwareBuffer::HBL_NORMAL);
      Ogre::uint8 * returnData = static_cast<Ogre::uint8*>(returnBufferPixelBox.data);
      std::cout << meanVec << std::endl;

      size_t pixelCounter = 0;
      for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
          std::size_t index = 4*(width*j + i);

          if (returnData[index] == 0 && returnData[index + 1] == 255 && returnData[index + 2] == 0) {
            continue;
          }
          Ogre::ColourValue color((float)returnData[index + 2] / 255.0f, (float)returnData[index + 1] / 255.0f, (float)returnData[index] / 255.0f);
          BGRtoLAlphaBeta(color);
          if (i == 1 && j == 0)
            std::cout << meanVec << std::endl;
            //std::cout << "r: " << color.r << "g: " << color.g << "b: " << color.b << std::endl;
          meanVec[2] += color.b; //blue
          meanVec[1] += color.g; //green
          meanVec[0] += color.r; //red
          pixelCounter++;
        }
      }
      returnBuffer->unlock();
      meanVec[0] = meanVec[0] / pixelCounter;
      meanVec[1] = meanVec[1] / pixelCounter;
      meanVec[2] = meanVec[2] / pixelCounter;
      return meanVec;
    };

#define POW2(x) ((x)*(x))

    Ogre::Vector3 computeVariance(Ogre::TexturePtr texture, Ogre::Vector3 mean) {
      Ogre::Vector3 varianceVec(0, 0, 0);
      size_t width = texture->getWidth();
      size_t height = texture->getHeight();
      Ogre::Image::Box imageBox;
      Ogre::HardwarePixelBufferSharedPtr returnBuffer = texture->getBuffer();
      const Ogre::PixelBox& returnBufferPixelBox = returnBuffer->lock(imageBox, Ogre::HardwareBuffer::HBL_NORMAL);
      Ogre::uint8 * returnData = static_cast<Ogre::uint8*>(returnBufferPixelBox.data);

      size_t pixelCounter = 0;
      for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            std::size_t index = 4 * (width*j + i);

            if (returnData[index] == 0 && returnData[index + 1] == 255 && returnData[index + 2] == 0) {
              continue;
            }
            Ogre::ColourValue color((float)returnData[index + 2], (float)returnData[index + 1], (float)returnData[index]);
            BGRtoLAlphaBeta(color);
            varianceVec[2] += POW2((color.b - mean[2])); //blue
            varianceVec[1] += POW2((color.g - mean[1])); //green
            varianceVec[0] += POW2((color.r - mean[0])); //red
            pixelCounter++;
        }
      }
      returnBuffer->unlock();
      varianceVec[0] = varianceVec[0] / pixelCounter;
      varianceVec[1] = varianceVec[1] / pixelCounter;
      varianceVec[2] = varianceVec[2] / pixelCounter;
      return varianceVec;
    };

    void AppWindow::computeColorBalancingParameter() {
      for (auto mapEntry : materials) {
        Ogre::MaterialPtr textureMaterial = mapEntry.second.matPtr;
        Ogre::Technique* technique = textureMaterial->getTechnique(0);
        Ogre::Pass* pass = technique->getPass(0);
        Ogre::GpuProgramParametersSharedPtr params = pass->getFragmentProgramParameters();

        params->setNamedConstant("meanInput", Ogre::Vector3(0, 0, 0));
        params->setNamedConstant("meanTarget", Ogre::Vector3(0, 0, 0));
        params->setNamedConstant("quotient", Ogre::Vector3(1, 1, 1));
      }
     
      if (frameCounter % 10 == 0){
        frameCounter = 0;
        renderTexture->update();
        meanInput = computeMean(rttTexture);
        meanTarget = computeMean(backgroundTexture);
        varianceInput = computeVariance(rttTexture, meanInput);
        varianceTarget = computeVariance(backgroundTexture, meanTarget);

        for (auto mapEntry : materials) {
          Ogre::MaterialPtr textureMaterial = mapEntry.second.matPtr;
          Ogre::Technique* technique = textureMaterial->getTechnique(0);
          Ogre::Pass* pass = technique->getPass(0);
          Ogre::GpuProgramParametersSharedPtr params = pass->getFragmentProgramParameters();

          std::cout << "Input Mean: \t\t" << meanInput << std::endl;
          std::cout << "Target Mean: \t\t" << meanTarget << std::endl;
          std::cout << "Input Variance: \t" << varianceInput << std::endl;
          std::cout << "Target Variance: \t" << varianceTarget << std::endl;
          std::cout << "Quotient: \t\t" << varianceTarget / varianceInput << std::endl;

          params->setNamedConstant("meanInput", meanInput);
          params->setNamedConstant("meanTarget", meanTarget);
          params->setNamedConstant("quotient", varianceTarget / varianceInput);
        }
        frameCounter++;
      }
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

      //* FIXME check functionality of above and remove this
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

          /*pDest++ = frameData[0]; frameData++;
          *pDest++ = frameData[0]; frameData++;
          *pDest++ = frameData[0]; frameData++;
          *pDest++=127;*/
        }

        pDest += pixelBox.getRowSkip() *
                 Ogre::PixelUtil::getNumElemBytes(pixelBox.format);

      }//*/

      // Unlock the pixel buffer
      pixelBuffer->unlock();
    }

    void AppWindow::update(
        float timeStep, imageProcessing::CameraStream* stream,
        const glm::mat4x4& viewMatrix) {
      glfwPollEvents();

      //const float* vmPtr = glm::value_ptr(viewMatrix);
      Ogre::Matrix4 matrix = Ogre::Matrix4(viewMatrix[0][0], viewMatrix[0][1], viewMatrix[0][2], viewMatrix[0][3],
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

      //we need this ...
      Ogre::WindowEventUtilities::messagePump();
    }

    bool AppWindow::isClosed() {
      return (glfwWindowShouldClose(glfwWindow)==GLFW_TRUE);
      //return renderWindow->isClosed();
    }

    Ogre::SceneManager* AppWindow::getSceneMgr() {
      return sceneMgr;
    }

    std::string AppWindow::createName() {
      return nameGenerator.generate();
    }

    void AppWindow::setWindowHint(std::string hint) {
      auto title=std::string("Ape!")+
                 std::string((hint=="")?"":" - ")+
                 hint;
      glfwSetWindowTitle(glfwWindow,title.c_str());
    }

    void
    AppWindow::processKeyEvent(int key, int scancode, int action, int mods) {
      keyEventHandler.callExceptIfNotSet(key,scancode,action,mods);
    }

    void AppWindow::processMousePositionEvent(double x, double y) {
      mousePositionEventHandler.callExceptIfNotSet(x, y);
      mousePosX = x;
      mousePosY = y;
    }

    void AppWindow::processMouseButtonEvent(int button, int action, int mods) {
      mouseButtonEventHandler.callExceptIfNotSet(button, action, mods);
      castViewPortRay(glm::vec2(mousePosX/1024.0f, mousePosY/768.0f));
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
      auto materialName= textureName+"_material";

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
      Ogre::MaterialPtr material = baseMaterial->clone(materialName);
      material->getTechnique(0)->getPass(0)->getTextureUnitState("DiffuseMap")->setTextureName(textureName);
      material->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CullingMode::CULL_NONE);

      InternalMaterial iMat;
      iMat.matPtr=material;
      iMat.texPtr=ogreTexture;
      materials[name]=iMat;

      return materialName;
    }

    Ogre::MaterialPtr AppWindow::getTextureName(const std::string surface) {
      return materials[surface].matPtr;
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
        Ogre::Entity* entity = static_cast<Ogre::Entity*>(resultObj->getParentSceneNode()->getAttachedObject(0));
        entitySelectionEventHandler.callExceptIfNotSet(entity,subIndex);
      } else {
        entitySelectionEventHandler.callExceptIfNotSet(nullptr,-1);
      }
    }

    Ogre::Entity* AppWindow::loadModel(std::string modelFile) {
      Ogre::Entity* worldEntity = sceneMgr->createEntity("world",modelFile);
      worldEntity->addQueryFlags(WORLD_OBJECT);
      worldEntity->setMaterial(materials["BaseWhite"].matPtr);

      //FIXME do not create & attach to node here
      Ogre::SceneNode* ogreNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
      //FIXME we use this only for loading the world, so we can for now
      // hard-code the transformation...
      ogreNode->rotate(
          Ogre::Vector3(1.0, 0.0, 0.0),
          Ogre::Radian(1.5707963268),
          Ogre::Node::TS_LOCAL);
#define scale 0.007
      ogreNode->setPosition(0.07, 0.00, 0.00);
      ogreNode->setScale(scale, scale, scale);
      ogreNode->attachObject(worldEntity);

      return worldEntity;
    }

  }
}