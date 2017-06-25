#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>
#include <string>

#include "../sections/appState/AppStateController.h"

#include "../../visualization/component/visualization.h"
#include "../../imageProcessing/component/imageProcessing.h"
#include "../3rdparty/args.h"

#include "opencv2/aruco.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace ape {

}

// Command line options
// for complete reference example, see
// https://github.com/magcks/hotools/blob/master/args.example.cc
struct Conf {
  std::string intrinsics;

  inline Conf(int argc, char **argv) : intrinsics("out")
  {
    args::parser args(argc, argv, "Ape desktop demo scene.");
    const int ARG_INTR  = args.add_opt("intrinsics",  "Camera intrinics file");

    for (int arg = args.next(); arg != args::parser::end; arg = args.next())
    {
      if (arg == ARG_INTR) intrinsics = args.val<std::string>();
      // else if (arg == ...
    }
  }
};

struct DistCoeffs {
	float coeffs[5];
};

//FIXME move to helper class
static void readCameraParameters(
  std::string filename, glm::mat3x3& camMatrix, DistCoeffs& distCoeffs) {
  std::ifstream fs(filename);
  if (!fs) {
    throw std::runtime_error("Could not read camera parameters");
  }

  std::string line;

  //read and discard image dimensions
  std::getline(fs, line);

  //read whitespace separated matrix values
  if (std::getline(fs, line)) {
    std::istringstream in(line);

    for (auto i = 0; i < 9; i++) {
      float f;
      in >> f;
      camMatrix[i / 3][i % 3] = f;
    }
  } else {
    throw std::runtime_error("invalid file format - A");
  }

  //read 5 whitespace separated distortion coefficients
  if (std::getline(fs, line)) {
    std::istringstream in(line);

    for (auto i = 0; i < 5; i++) {
      float f;
      in >> f;
      distCoeffs.coeffs[i] = f;
    }
  } else {
    throw std::runtime_error("invalid file format - B");
  }
}


int main(int argc, char** argv) {
  Conf conf(argc, argv);
  std::cout << "Camera intrinsics file:  " << conf.intrinsics << std::endl;

  //setup image processing
  glm::mat3x3 camMatrix;
  DistCoeffs distCoeffs;
  //FIXME magic string
  readCameraParameters(conf.intrinsics, camMatrix, distCoeffs);

  auto  ipController=
      ape::imageProcessing::IImageProcessingController::createInstance(
          camMatrix, &distCoeffs.coeffs[0]);
  auto camStream = ipController->getCameraStream();

  //setup visualization
  auto visController=
      ape::visualization::IVisualizationController::createInstance(camStream);
  visController->setProjectionMatrix(camMatrix);

  ape::app::desktop::section::appState::AppStateController appStateController(
      ipController.get(),
      visController.get()
  );

  visController->overlayChangeRequestHandler.setCallback([](
      void* appStateController,
      ape::visualization::IVisualizationController::Overlay overlay) -> void {
    auto asc=(ape::app::desktop::section::appState::AppStateController*)appStateController;
    switch (overlay) {
      case ape::visualization::IVisualizationController::Overlay::Loading:
        asc->requestTransition(
            ape::app::desktop::section::appState::AppStateController::State::LoadingScreen
        );
        break;
      case ape::visualization::IVisualizationController::Overlay::Menu:
        asc->requestTransition(
            ape::app::desktop::section::appState::AppStateController::State::MainMenu
        );
        break;
      case ape::visualization::IVisualizationController::Overlay::WorldScreen:
        asc->requestTransition(
            ape::app::desktop::section::appState::AppStateController::State::WorldScreen
        );
        break;
      case ape::visualization::IVisualizationController::Overlay::TextureSynthesisSelection:
        asc->requestTransition(
            ape::app::desktop::section::appState::AppStateController::State::TextureSynthesisSelection
        );
        break;
    }
  },&appStateController);

  //application loop
  //FIXME refactor into separate class
  auto frameTime = 1.0f / 30.0f;
  while (!visController->getTerminateRequest()) {
    auto frameStart = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now()
    );

    //update controllers handling incoming data
    ipController->update(frameTime);

    //update internal app state
    appStateController.update(frameTime);

    //update visualization
    visController->update(frameTime);


    auto frameEnd = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now()
    );
    auto duration=(frameEnd-frameStart).count() / 1000.0;

    if (duration<frameTime) {
      auto remainingFrameTime=frameTime-duration;
      std::this_thread::sleep_for(std::chrono::milliseconds(
          (int)(remainingFrameTime * 1000)));
    }
  }

  std::cout << "Terminated" << std::endl;
  return 0;
}
