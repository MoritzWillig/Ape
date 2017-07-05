#include <fstream>
#include "SurfaceImplementation.h"

namespace ape {
  namespace worldState {

    SurfaceImplementation::SurfaceImplementation():
        path(), handle(), name(), texture() {
    }

    SurfaceImplementation::SurfaceImplementation(
    std::string path, ISurface::SurfacePersistentHandle handle,
    std::string name):
    path(path), isExisting(), handle(handle), name(name) {
      probeSurfaceExisting();
    }

    void SurfaceImplementation::probeSurfaceExisting() {
      if (path.hasSignal()) {
        std::fstream surfaceStream(path.getValue(), std::ios_base::in);
        isExisting.set(static_cast<bool>(surfaceStream));
      }
    }

    std::string SurfaceImplementation::getName() {
      return name;
    }

    ValueSignal<ISurface::SurfacePersistentHandle>
    SurfaceImplementation::getPersistentHandle() {
      return handle;
    }

    cv::Mat SurfaceImplementation::getTexture() {
      return texture;
    }

    void SurfaceImplementation::setTexture(cv::Mat texture) {
      texture.copyTo(this->texture);
    }

  }
}
