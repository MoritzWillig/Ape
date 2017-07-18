#include <iostream>
#include <sstream>
#include <fstream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/aruco/charuco.hpp"

#include "../application/imageProcessing/component/imageTools.h"
#include "../3rdparty/args.h"


struct Conf {
  std::string inPath;
  std::string outPath;

  inline Conf(int argc, char **argv)
  {
    args::parser args(argc, argv, "Texture Synthesis");

    const int ARG_IN  = args.add_nonopt("IN-IMAGE-PATH");
    const int ARG_OUT  = args.add_nonopt("OUT-IMAGE-PREFIX");
    args.range(2);

    for (int arg = args.next(); arg != args::parser::end; arg = args.next())
    {
      if (arg == ARG_IN) inPath = args.val<std::string>();
      else if (arg == ARG_OUT) outPath = args.val<std::string>();
    }
  }
};


int main(int argc, char** argv) {
  Conf conf(argc, argv);

  // read image, format is BGR (CV Default)
  cv::Mat image = cv::imread(conf.inPath);

  cv::Mat outImage = image.clone();
  //ape::imageProcessing::equalizeHistogram(outImage);
  ape::imageProcessing::robustAWB(outImage);

  std::stringstream outName;
  outName << conf.outPath << ".png";
  std::cout << "Saving image to: " << outName.str() << std::endl;

  std::vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(9);
  cv::imwrite(outName.str(), outImage, compression_params);

  return 0;
}
