#include <iostream>
#include <sstream>
#include <fstream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/aruco/charuco.hpp"

// this didn't work for some reason...
//#include "imageProcessing/TextureSynthesis.h"
#include "../../../../components/imageProcessing/TextureSynthesis.h"
#include "../3rdparty/args.h"


struct Conf {
  std::string inPath;
  std::string outPath;
  std::size_t width;
  std::size_t height;

  inline Conf(int argc, char **argv) : width(512), height(512)
  {
    args::parser args(argc, argv, "Texture Synthesis");

    const int ARG_IN  = args.add_nonopt("IN-IMAGE-PATH");
    const int ARG_OUT  = args.add_nonopt("OUT-IMAGE-PREFIX");
    args.range(2);
    const int ARG_W  = args.add_opt('w', "width", "Texture width");
    const int ARG_H  = args.add_opt('h', "height", "Texture height");

    for (int arg = args.next(); arg != args::parser::end; arg = args.next())
    {
      if (arg == ARG_IN) inPath = args.val<std::string>();
      else if (arg == ARG_OUT) outPath = args.val<std::string>();
      else if (arg == ARG_W) width = args.val<std::size_t>();
      else if (arg == ARG_H) height = args.val<std::size_t>();
    }
  }
};


int main(int argc, char** argv) {
  Conf conf(argc, argv);
  std::cout << "Synthesising texture..." << std::endl;

  // read image, format is BGR (CV Default)
  cv::Mat image = cv::imread(conf.inPath);

  ape::imageProcessing::TextureSynthesis ts(image,
    conf.width, conf.height);

  cv::Mat texture(conf.width, conf.height, CV_8UC3);
  ts.generateTexture(3, texture);

  std::vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(9);

  std::stringstream outName;
  outName << conf.outPath << ".png" << std::endl;
  std::cout << "Saving Texture in: " << outName.str() << std::endl;
  cv::imwrite(outName.str(), texture, compression_params);

  return 0;
}
