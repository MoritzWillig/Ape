#include <iostream>

#include "../../component/visualization.h"

namespace ape {

}

int main(int argc, char** argv) {
  std::cout<<"Demo scene"<<std::endl;

  ape::visualization::VisualizationController controller;
  controller.startDisplay();

  //FIXME refactor into separate class
  auto frameTime=1.0f / 30.0f;
  while (!controller.getTerminateRequest()) {
    controller.update(frameTime);
    //usleep((unsigned int)frameTime*1000*1000);
  }

  std::cout<<"Terminated"<<std::endl;

  return 0;
}