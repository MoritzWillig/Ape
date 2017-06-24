#include <stdexcept>
#include "TextureExtraction.h"

namespace ape {
  namespace imageProcessing {

    TextureExtraction::TextureExtraction() {

    }

    cv::Mat TextureExtraction::extractRegionOfInterest(const cv::Mat & frame,
      const cv::Rect & roi) {
      return frame(roi);
    }
  }
}
