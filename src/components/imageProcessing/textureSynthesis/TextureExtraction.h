#pragma once

#include <imageProcessing/CameraStream.h>

#include "opencv2/videoio.hpp"

namespace ape {
  namespace imageProcessing {

    class TextureExtraction {
    private:
    protected:
    public:
      //Default
      TextureExtraction();

      // Copy constructor
      TextureExtraction(const TextureExtraction&) = default;

      // Move constructor
      TextureExtraction(TextureExtraction&&) = default;

      // Copy assignment operator
      TextureExtraction& operator=(const TextureExtraction&)& = default;

      // Move assignment operator
      TextureExtraction& operator=(TextureExtraction&&)& = default;

      // Destructor
      ~TextureExtraction() = default;

      cv::Mat extractRegionOfInterest(const cv::Mat &frame, const cv::Rect &roi);
    };

  }
}


