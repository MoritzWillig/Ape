#include <fstream>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
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

      std::cout<<"Loading surface "<<path<<std::endl;
      if (isExisting.hasSignal()) {
        try {
          texture = cv::imread(path, cv::IMREAD_COLOR);
          std::cout<<"Loaded surface "<<name<<std::endl;
        } catch (std::exception e) {
          std::cout<<"Could not load surface "<<name<<std::endl
                   <<e.what()<<std::endl;
          isExisting.reset();
        }
      }

      //FIXME set dummy texture
      if (isExisting.hasSignal()) {

      }
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
