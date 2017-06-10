
#include <OGRE/OgreRenderOperation.h>
#include "OgreShapes.h"

namespace ape {
  namespace visualization {
    namespace shapes {

      OgrePolygon2D::OgrePolygon2D(AppWindow* appWindow,
                                   std::vector<glm::vec2> shape) :
          appWindow(appWindow), shape(shape),
          ogreObject(appWindow->getSceneMgr()->createManualObject()) {
        ogreObject->setUseIdentityProjection(true);
        ogreObject->setUseIdentityView(true);
        updateOgreObject();
      }

      void OgrePolygon2D::setVisible(bool visible) {
        ogreObject->setVisible(visible);
      }

      Ogre::MovableObject* OgrePolygon2D::getOgreObject() {
        return ogreObject;
      }

      void OgrePolygon2D::updateOgreObject() {
        ogreObject->begin("OverlayButton",
                          Ogre::RenderOperation::OT_TRIANGLE_STRIP);

        ogreObject->colour(1.0f,0.6f,0.0f);

        for (auto vertex: shape) {
          ogreObject->position(vertex.x, vertex.y, 0.0f);
          ogreObject->textureCoord(vertex.x*0.5f, vertex.y*0.5f);
        }

        for(unsigned int i=0; i<shape.size(); i++) {
          ogreObject->index(i);
        }
        ogreObject->index(0);

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


    }
  }
}