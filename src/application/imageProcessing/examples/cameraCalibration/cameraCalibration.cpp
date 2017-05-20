
#include <iostream>
#include "opencv2/aruco.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"

namespace ape {

}

int main(int argc, char** argv) {
  // start video capture
  // -1 gets any camera
  cv::VideoCapture cap(-1);

  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(
      cv::aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_250));

  while(cap.isOpened()) {
    cv::Mat frame, frameCopy;
    if (!cap.read(frame)) {
      std::cerr << "Could not read frame from camera." << std::endl;
      break;
    }

    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> corners, rejected;
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();

    // detect markers
    cv::aruco::detectMarkers(frame, dictionary, corners, ids, detectorParams,
                             rejected);

    frame.copyTo(frameCopy);
    std::cout << rejected.size() << std::endl;
    if (!ids.empty()) {
      cv::aruco::drawDetectedMarkers(frameCopy, corners, ids);
    }

    cv::imshow("Aruco test", frameCopy);
    int k = cv::waitKey(33);
    if (k==27) {
      break;
    }
  }


  return 0;
}
