#pragma once

#include <cmath>
#include "opencv2/highgui.hpp"

#include "math_defines.h"

namespace ape {
  namespace imageProcessing {
    namespace colorSpaces {

      struct RGB;
      struct LAB;

      class RGB {
        public:
          RGB(float r, float g, float b) : r(r), g(g), b(b) {}
          float r, g, b;

          LAB toLAB();
      };

      class LAB {

        public:
          LAB(float l, float a, float b): l(l), a(a), b(b) {}
          float l, a, b;
      };


      void BGR2YUV(cv::Mat const& image, cv::Mat &ret);

    } // end namespace
  }
}
