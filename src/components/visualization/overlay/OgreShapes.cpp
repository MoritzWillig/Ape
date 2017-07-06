
#include <OGRE/OgreRenderOperation.h>
#include "OgreShapes.h"

namespace ape {
  namespace visualization {
    namespace shapes {

      OgrePolygon2D::OgrePolygon2D(AppWindow* appWindow,
                                   std::vector<glm::vec2> shape,
                                   Ogre::ColourValue colour) :
          appWindow(appWindow), shape(shape),
          ogreObject(nullptr),
          colour(colour), textureName() {
        regenerateOgreObject();
        updateOgreObject();
      }

      OgrePolygon2D::~OgrePolygon2D() {
        if (ogreObject!=nullptr) {
          appWindow->getSceneMgr()->destroyManualObject(ogreObject);
        }
      }

      void OgrePolygon2D::setVisible(bool visible) {
        ogreObject->setVisible(visible);
      }

      Ogre::MovableObject* OgrePolygon2D::getOgreObject() {
        return ogreObject;
      }

      void OgrePolygon2D::updateOgreObject() {
        regenerateOgreObject();

        ogreObject->begin("OverlayButton",
                          Ogre::RenderOperation::OT_TRIANGLE_FAN);
        ogreObject->colour(colour);
        if (textureName.hasSignal()) {
          ogreObject->setMaterialName(0,textureName.getValue());
        }

        for (auto vertex: shape) {
          ogreObject->position(vertex.x, vertex.y, 0.0f);
          ogreObject->textureCoord(vertex.x, vertex.y);
        }

        for(unsigned int i=0; i<shape.size(); i++) {
          ogreObject->index(i);
        }

        ogreObject->end();


        // Render just before overlays
        ogreObject->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);

        // Use infinite AAB to always stay visible
        Ogre::AxisAlignedBox aabInf;
        aabInf.setInfinite();
        ogreObject->setBoundingBox(aabInf);

        ogreObject->setVisible(true);

        appWindow->getSceneMgr()->getRootSceneNode()->createChildSceneNode()
            ->attachObject(ogreObject);
      }

      void OgrePolygon2D::regenerateOgreObject() {
        if (ogreObject!=nullptr) {
          appWindow->getSceneMgr()->destroyManualObject(ogreObject);
        }
        ogreObject=appWindow->getSceneMgr()->createManualObject();
        ogreObject->setUseIdentityProjection(true);
        ogreObject->setUseIdentityView(true);
      }


    }
  }
}