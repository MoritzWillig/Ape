#include <iostream>
#include <chrono>
#include <thread>

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
		std::string filename, glm::mat4x4& camMatrix, DistCoeffs& distCoeffs) {
	std::stringstream ss(filename, std::ios_base::in);

	if (!ss) {
		throw new std::runtime_error("Could not read camera parameters");
	}

  //fixme use external format parser
	for (auto i=0; i<16; i++) {
		float* f;
		ss>>f;
		camMatrix[i]=f;

    //read space
    ss.read();
	}

  for (auto i=0; i<5; i++) {
    float* f;
    ss>>f;
    distCoeffs.coeffs[i]=f;

    //read space
    ss.read();
  }
}

int main(int argc, char** argv) {
	std::cout << "Demo scene" << std::endl;

  //setup image processing
  glm::mat4 camMatrix;
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