
#include <OGRE/OgreRenderOperation.h>
#include "OgreShapes.h"

namespace ape {
  namespace visualization {
    namespace shapes {

      OgrePolygon2D::OgrePolygon2D(AppWindow* appWindow,
                                   std::vector<glm::vec2> shape) :
          appWindow(appWindow), shape(),
          ogreObject(appWindow->getSceneMgr()->createManualObject()) {
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

      }


    }
  }
}