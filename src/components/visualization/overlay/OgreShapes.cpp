
#include <OGRE/OgreRenderOperation.h>
#include "OgreShapes.h"

namespace ape {
  namespace visualization {
    namespace shapes {

      OgrePolygon2D::OgrePolygon2D(AppWindow* appWindow,
                                   std::vector<glm::vec2> shape) :
          appWindow(appWindow), shape(shape),
          ogreObject(appWindow->getSceneMgr()->createManualObject()) {
        appWindow->getSceneMgr()->getRootSceneNode()->attachObject(ogreObject);
        updateOgreObject();
      }

      void OgrePolygon2D::setVisible(bool visible) {
        ogreObject->setVisible(visible);
      }

      Ogre::MovableObject* OgrePolygon2D::getOgreObject() {
        return ogreObject;
      }

      void OgrePolygon2D::updateOgreObject() {
        ogreObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

        ogreObject->colour(1.0f,0.8f,0.0f);

        for (auto vertex: shape) {
          ogreObject->position(vertex.x, vertex.y, 0.0f);
        }

        for(unsigned int i=0; i<shape.size(); i++) {
          ogreObject->index(i);
        }
        ogreObject->index(0);

        ogreObject->end();

        // get the overlay manager
        Ogre::OverlayManager& overlayMgr = Ogre::OverlayManager::getSingleton();
        Ogre::Overlay* overlay = overlayMgr.getByName("GUI");

        // Create a panel
        Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(
            overlayMgr.createOverlayElement("Panel", "PanelName2"));
        panel->setMetricsMode(Ogre::GMM_PIXELS);
        panel->setPosition(10, 150);
        panel->setDimensions(300, 120);
        panel->setColour(Ogre::ColourValue(0.8,0.6,0.0,0.5));
        panel->setMaterialName("Background");

        // Create a text area
        Ogre::TextAreaOverlayElement* textArea = static_cast<Ogre::TextAreaOverlayElement*>(
            overlayMgr.createOverlayElement("TextArea", "TextAreaName2"));
        textArea->setMetricsMode(Ogre::GMM_PIXELS);
        textArea->setPosition(0, 0);
        textArea->setDimensions(300, 120);
        textArea->setCharHeight(26);
        // set the font name to the font resource that you just created.
        textArea->setFontName("MyFont");
        // say something
        textArea->setCaption("Hello, World!");

        // Create an overlay, and add the panel
        overlay->add2D(panel);

        // Add the text area to the panel
        //panel->addChild(textArea);

        // Show the overlay
        overlay->show();
      }


    }
  }
}