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

static bool readCameraParameters(std::string filename, cv::Mat &camMatrix, cv::Mat &distCoeffs) {
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	if (!fs.isOpened())
		return false;
	fs["camera_matrix"] >> camMatrix;
	fs["distortion_coefficients"] >> distCoeffs;
	return true;
}

int main(int argc, char** argv) {
	std::cout << "Demo scene" << std::endl;

  //setup visualization
	ape::visualization::VisualizationController visController;
	visController.startDisplay();

  //setup image processing
  //FIXME as glm::mat4 and float[]
  cv::Mat camMatrix_, distCoeffs_; //FIXME remove this
  glm::mat4 camMatrix; //FIXME read into this
  float distCoeffs[5]; //FIXME and this
  readCameraParameters("out", camMatrix_, distCoeffs_);

  ape::imageProcessing::ImageProcessingController ipController(
      camMatrix, distCoeffs);
  auto camStream = ipController.getCameraStream();

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


		visController.update(
        frameTime, frame.data, frame.size().width,frame.size().height,
        viewTransform);
		std::this_thread::sleep_for(std::chrono::microseconds((int)frameTime * 1000));
	}

	std::cout << "Terminated" << std::endl;

	return 0;
}