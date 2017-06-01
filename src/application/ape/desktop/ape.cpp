#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>

#include "../../visualization/component/visualization.h"
#include "../../imageProcessing/component/imageProcessing.h"

#include "opencv2/aruco.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace ape {

}

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
	std::cout << "Demo scene" << std::endl;

  //setup image processing
  glm::mat3x3 camMatrix;
  DistCoeffs distCoeffs;
  //FIXME magic string
  readCameraParameters("out", camMatrix, distCoeffs);

  ape::imageProcessing::ImageProcessingController ipController(
      camMatrix, &distCoeffs.coeffs[0]);
  auto camStream = ipController.getCameraStream();

  //setup visualization
  ape::visualization::VisualizationController visController(camStream);
  visController.startDisplay();


  //application loop
	//FIXME refactor into separate class
	auto frameTime = 1.0f / 30.0f;
	while (!visController.getTerminateRequest()) {
    ipController.update(frameTime);

    auto worldVisible = ipController.hasMarker();
    auto viewTransform = ipController.getTransformation();

    //FIXME C-cast ...
    //FIXME we need to set a type, width & height?
    cv::Mat frame(1,1,CV_8UC3,camStream->getCurrentFrame());

    //FIXME if worldVisible display world. See #5 for filter description

    visController.update(frameTime, viewTransform);
		std::this_thread::sleep_for(std::chrono::microseconds((int)frameTime * 1000));
	}

	std::cout << "Terminated" << std::endl;

	return 0;
}