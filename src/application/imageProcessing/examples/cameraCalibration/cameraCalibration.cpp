#include <iostream>
#include <fstream>

#include "opencv2/aruco.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/aruco/charuco.hpp"

#include "../../component/imageProcessing.h"

#include "imageProcessing/CameraStream.h"

namespace ape {

}


// FIXME: Refactor this to be in the same file as the function which reads
// in the parameters
int saveCameraIntrinsics(std::string filename, cv::Size imageSize,
  cv::Mat const& cameraMatrix, cv::Mat const& distCoeffs, double totalAvgError)
{
  std::cout << "Saving camera intrinsics to " << filename << std::endl;
  std::ofstream out(filename);
  if (!out.is_open()) {
    std::cerr << "Unable to open " << filename << std::endl;
    return EXIT_FAILURE;
  }

  out << imageSize.width << " " << imageSize.height << "\n";

  // write camera matrix (3x3)
  for (std::size_t r = 0; r < cameraMatrix.rows; ++r)
    for (std::size_t c = 0; c < cameraMatrix.cols; ++c)
      out << cameraMatrix.at<double>(r,c) << " ";
  out << "\n";

  // write distortion 5 parameters
  for (std::size_t r = 0; r < distCoeffs.rows; ++r)
    for (std::size_t c = 0; c < distCoeffs.cols; ++c)
      out << distCoeffs.at<double>(r,c) << " ";
  out << "\n";

  out << totalAvgError << "\n";

  out.close();
  return EXIT_SUCCESS;
}



int main(int argc, char** argv) {
  std::cout << "Camera calibration using Charuko marker board" << std::endl;

  // create a charuko marker board
  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(
    cv::aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_50));
  cv::Ptr<cv::aruco::CharucoBoard> board = cv::aruco::CharucoBoard::create(
    8, 5, 1.0f, 0.8f, dictionary);

#if 0

  cv::Size boardSize(1920, 1080);
  cv::Mat boardImage;
  board->draw(boardSize, boardImage);

  cv::imwrite("/tmp/board.jpg", boardImage);
#endif


  std::vector<std::vector<std::vector<cv::Point2f>>> allCorners;
  std::vector<std::vector<int>> allIds;
  std::vector<cv::Mat> allImgs;

  auto controller = ape::imageProcessing::IImageProcessingController::createInstance(
        glm::mat3(), nullptr);
  ape::imageProcessing::CameraStream * camera = controller->getCameraStream();

   while (true) {
    if (allImgs.size() > 3) break;


    cv::Mat frame(camera->getFrameHeight(), camera->getFrameWidth(), CV_8UC3);
    controller->update(0);
    frame.data = (uchar*) camera->getCurrentFrame().data;

    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> corners, rejected;
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();

    // detect markers
    cv::aruco::detectMarkers(frame, dictionary, corners, ids, detectorParams,
      rejected);

    // interpolate charuco corners
    cv::Mat currentCharucoCorners, currentCharucoIds;
    if(ids.size() > 0) {
      cv::aruco::interpolateCornersCharuco(corners, ids, frame, board,
        currentCharucoCorners, currentCharucoIds);
    }

    cv::Mat frameCopy;
    frame.copyTo(frameCopy);
    if (!ids.empty()) {
      cv::aruco::drawDetectedMarkers(frameCopy, corners, ids);
    }

    if(currentCharucoCorners.total() > 0) {
      cv::aruco::drawDetectedCornersCharuco(frameCopy, currentCharucoCorners,
        currentCharucoIds);
    }


    cv::imshow("Aruco test", frameCopy);
    int k = cv::waitKey(33);
    if (k==27) {
      break;
    }
    else if (k == 'c' && !ids.empty()) {
      std::cout << "Captured " << ids.size() << " ids and " << corners.size()
        << " corners " << std::endl;

      // capture this frame and detections
      allCorners.push_back(corners);
      allIds.push_back(ids);
      allImgs.push_back(frame);
    }
  }

  cv::Size cameraImageSize(camera->getFrameWidth(),
    camera->getFrameHeight());


  std::cout << "Total ids: " << allIds.size() << " total corners: "
    << allCorners.size() << std::endl;

  // prepare data for charuco calibration
  cv::Mat cameraMatrix, distCoeffs;
  std::vector<cv::Vec3d> rvecs, tvecs;
  int nFrames = (int)allCorners.size();
  std::vector<cv::Mat> allCharucoCorners;
  std::vector<cv::Mat> allCharucoIds;
  allCharucoCorners.reserve(nFrames);
  allCharucoIds.reserve(nFrames);

  for(std::size_t i = 0; i < nFrames; i++) {
    // interpolate using camera parameters
    cv::Mat currentCharucoCorners, currentCharucoIds;
    cv::aruco::interpolateCornersCharuco(allCorners[i], allIds[i], allImgs[i],
      board, currentCharucoCorners, currentCharucoIds, cameraMatrix,
      distCoeffs);

    allCharucoCorners.push_back(currentCharucoCorners);
    allCharucoIds.push_back(currentCharucoIds);
  }

  if(allCharucoCorners.size() < 4) {
    std::cerr << "Not enough corners for calibration" << std::endl;
    return EXIT_FAILURE;
  }

  // calibrate
  int calibrationFlags = 0;
  double repError = cv::aruco::calibrateCameraCharuco(allCharucoCorners,
    allCharucoIds, board, cameraImageSize, cameraMatrix, distCoeffs, rvecs, tvecs,
    calibrationFlags);

  //FIX ME magic string
  saveCameraIntrinsics("out", cameraImageSize, cameraMatrix,
    distCoeffs, repError);

  std::cout << "Calibration reprojection errror: " << repError << std::endl;

  while (true) {
    cv::Mat frame(camera->getFrameHeight(),
      camera->getFrameWidth(), CV_8UC3);
    controller->update(0);
    frame.data = (uchar*) camera->getCurrentFrame().data;

    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> corners, rejected;
    std::vector<cv::Vec3d> rvecs, tvecs;
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();

    // detect markers
    cv::aruco::detectMarkers(frame, dictionary, corners, ids, detectorParams,
      rejected);

    cv::aruco::estimatePoseSingleMarkers(corners, 0.026f, cameraMatrix,
      distCoeffs, rvecs, tvecs);

    cv::Mat frameCopy;
    frame.copyTo(frameCopy);
    if (!ids.empty()) {
      cv::aruco::drawDetectedMarkers(frameCopy, corners, ids);
    }

    for (std::size_t i = 0; i < ids.size(); ++i) {
      cv::aruco::drawAxis(frameCopy, cameraMatrix, distCoeffs, rvecs.at(i),
        tvecs.at(i), 0.026f);
    }

    cv::imshow("Aruco test", frameCopy);
    int k = cv::waitKey(33);
    if (k==27) {
      break;
    }
  }


  return 0;
}
