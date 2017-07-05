// from http://www.ogre3d.org/tikiwiki/Raycasting+to+the+polygon+level

#pragma once

#include <OGRE/Ogre.h>
#include <OGRE/OgreVector3.h>

namespace ape {
  namespace visualization {

    class OgreRay
    {
    private:
      Ogre::SceneManager* sceneMgr;
      Ogre::RaySceneQuery* m_raySceneQuery;//!< Ray query

    public:
      OgreRay(Ogre::SceneManager* sceneMgr);
      ~OgreRay();
      bool Raycast(Ogre::Ray& ray, Ogre::Vector3& result, Ogre::MovableObject** resultObj, size_t& resultSubIdx);

    private:
      static void GetMeshInformation(const Ogre::MeshPtr mesh, size_t &vertex_count, Ogre::Vector3*& vertices, size_t& index_count, unsigned long*& indices, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale);
      static void GetMeshInformation(const Ogre::ManualObject* manual, size_t& vertex_count, Ogre::Vector3*& vertices, size_t& index_count, unsigned long*& indices, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale);
      static void GetMeshInformation(const Ogre::Entity* entity, size_t& vertex_count, Ogre::Vector3*& vertices, size_t& index_count, unsigned long*& indices, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale);
      static void GetSubMeshInformation(const Ogre::MeshPtr mesh, const size_t subMeshIndex, size_t &vertex_count, Ogre::Vector3*& vertices, size_t& index_count, unsigned long*& indices, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale);
      static void GetSubMeshInformation(const Ogre::ManualObject* manual, const size_t subMeshIndex, size_t& vertex_count, Ogre::Vector3*& vertices, size_t& index_count, unsigned long*& indices, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale);
      static void GetSubMeshInformation(const Ogre::Entity* entity, const size_t subMeshIndex, size_t& vertex_count, Ogre::Vector3*& vertices, size_t& index_count, unsigned long*& indices, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale);
    };

  }
}