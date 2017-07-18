#pragma once

#include "opencv2/highgui.hpp"

namespace ape {
  namespace imageProcessing {

    // Performs automatic white balancing on the given image.
    //
    // Based on:
    //
    //   J. Huo, Y. Chang, J. Wang, and X. Wei, “Robust automatic white balance
    //   algorithm using gray color points in images,” IEEE Transactions on
    //   Consumer Electronics, vol. 52, no. 2, pp. 541–546, May 2006.
    void robustAWB(cv::Mat & image, float T = 0.1321,
      float gainIncrement = 0.01f, std::size_t maxIters = 100);

  } // namespace end
} // namespace end
