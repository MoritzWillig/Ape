#include <iostream>
#include <chrono>
#include <thread>

#include "../../visualization/component/visualization.h"
#include "../../imageProcessing/component/imageProcessing.h"

#include "opencv2/aruco.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d.hpp"

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

// Source: http://stackoverflow.com/questions/3712049/how-to-use-an-opencv-rotation-and-translation-vector-with-opengl-es-in-android
static cv::Mat convertVectorsToViewMatrix(cv::Vec3d rotation, cv::Vec3d translation) {
	// rotation matrix
	cv::Mat rotMat = cv::Mat::zeros(4, 4, CV_64F);
	cv::Mat viewMatrix = cv::Mat::zeros(4, 4, CV_64F);

	// rotation vectors can be converted to a 3-by-3 rotation matrix
	cv::Rodrigues(rotation, rotMat);

	//Complete matrix ready to use
	for (unsigned int row = 0; row<3; ++row)
	{
		for (unsigned int col = 0; col<3; ++col)
		{
			viewMatrix.at<double>(row, col) = rotMat.at<double>(row, col);
		}
		viewMatrix.at<double>(row, 3) = cv::Mat(translation).at<double>(row, 0);
	}
	//viewMatrix.at<double>(2, 3) = -viewMatrix.at<double>(2, 3);
	viewMatrix.at<double>(3, 3) = 1.0f;

	cv::Mat cvToGl = cv::Mat::zeros(4, 4, CV_64F);
	cvToGl.at<double>(0, 0) = 1.0f;
	cvToGl.at<double>(1, 1) = -1.0f;
	// Invert the y axis 
	cvToGl.at<double>(2, 2) = -1.0f;
	// invert the z axis 
	cvToGl.at<double>(3, 3) = 1.0f;
	viewMatrix = cvToGl * viewMatrix;

	viewMatrix.at<double>(0, 3) = cv::Mat(translation).at<double>(0, 0);
	viewMatrix.at<double>(1, 3) = -cv::Mat(translation).at<double>(1, 0);
	viewMatrix.at<double>(2, 3) = -cv::Mat(translation).at<double>(2, 0);

	printf("[%f %f %f %f\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f]\n",
		viewMatrix.at<double>(0), viewMatrix.at<double>(1), viewMatrix.at<double>(2), viewMatrix.at<double>(3),
		viewMatrix.at<double>(4), viewMatrix.at<double>(5), viewMatrix.at<double>(6), viewMatrix.at<double>(7),
		viewMatrix.at<double>(8), viewMatrix.at<double>(9), viewMatrix.at<double>(10), viewMatrix.at<double>(11),
		viewMatrix.at<double>(12), viewMatrix.at<double>(13), viewMatrix.at<double>(14), viewMatrix.at<double>(15));
	return viewMatrix;
}

int main(int argc, char** argv) {
	std::cout << "Demo scene" << std::endl;

	// start video capture
	// -1 gets any camera
	cv::VideoCapture cap(-1);
	cv::Mat frame;

	ape::visualization::VisualizationController visVontroller;
	visVontroller.startDisplay();


	cv::Mat camMatrix, distCoeffs;
	readCameraParameters("out", camMatrix, distCoeffs);

	//ape::imageProcessing::ImageProcessingController procController;
	//auto stream = procController.getCameraStream();

	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(
		cv::aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_250));


	float markerLength = 0.15;
	std::vector< int > ids;
	std::vector< std::vector< cv::Point2f > > corners, rejected;
	std::vector< cv::Vec3d > rvecs, tvecs;
	cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();

	cv::Mat viewMatrix = cv::Mat::zeros(4, 4, CV_64F);

	//FIXME refactor into separate class
	auto frameTime = 1.0f / 30.0f;
	while (!visVontroller.getTerminateRequest()) {

		if (!cap.read(frame)) {
			std::cerr << "Could not read frame from camera." << std::endl;
			break;
		}


		// detect markers and estimate pose
		cv::aruco::detectMarkers(frame, dictionary, corners, ids, detectorParams, rejected);
		cv::aruco::estimatePoseSingleMarkers(corners, markerLength, camMatrix, distCoeffs, rvecs,
				tvecs);


		if (ids.size() > 0) {
			cv::aruco::drawDetectedMarkers(frame, corners, ids);
			viewMatrix = convertVectorsToViewMatrix(rvecs[0], tvecs[0]);
			for (unsigned int i = 0; i < ids.size(); i++)
				cv::aruco::drawAxis(frame, camMatrix, distCoeffs, rvecs[i], tvecs[i],
					markerLength * 0.5f);
		}

		//cv::imshow("Aruco test", frame);
		visVontroller.update(frameTime, frame.data, frame.size().width, frame.size().height, &viewMatrix.at<double>(0, 0));
		std::this_thread::sleep_for(std::chrono::microseconds((int)frameTime * 1000));
	}

	std::cout << "Terminated" << std::endl;

	return 0;
}