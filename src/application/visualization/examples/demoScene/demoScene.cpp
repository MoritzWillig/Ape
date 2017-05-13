#include <iostream>

#include "../../component/visualization.h"

namespace ape {

}

int main(int argc, char** argv) {
  std::cout<<"Demo scene"<<std::endl;

  ape::visualization::VisualizationController controller;
  auto scene=controller.getScene();
  scene->startRendering();

  return 0;
}