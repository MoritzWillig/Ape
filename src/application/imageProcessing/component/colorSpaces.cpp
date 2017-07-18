#include "colorSpaces.h"

namespace ape {
  namespace imageProcessing {
    namespace colorSpaces {


      LAB RGB::toLAB()
      {
        float L = 0.3811 * this->r + 0.5783 * this->g + 0.0402 * this->b;
        float M = 0.1967 * this->r + 0.7244 * this->g + 0.0782 * this->b;
        float S = 0.0241 * this->r + 0.1288 * this->g + 0.8444 * this->b;

        // Workaround to fix undefined log of 0
        if (L == 0)
          L = 1 / 255.f;
        if (M == 0)
          M = 1 / 255.f;
        if (S == 0)
          S = 1 / 255.f;

        L = std::log(L);
        M = std::log(M);
        S = std::log(S);

        float l     = m_1_sqrt_3 * L + m_1_sqrt_3 * M + m_1_sqrt_3 * S;
        float alpha = m_1_sqrt_6 * L + m_1_sqrt_6 * M - 2 * m_1_sqrt_6 * S;
        float beta  = m_1_sqrt_2 * L - m_1_sqrt_2 * M;

        return LAB(l, alpha, beta);
      }


      // ----------------------------------------------------------------------
      // whole image conversions
      //-----------------------------------------------------------------------
      void BGR2YUV(cv::Mat const& image, cv::Mat & ret)
      {
        ret.create(image.rows, image.cols, CV_32FC3);
        for (int y = 0; y < image.rows; ++y) {
          for (int x = 0; x < image.cols; ++x) {

              unsigned char b = image.at<cv::Vec3b>(y, x)[0];
              unsigned char g = image.at<cv::Vec3b>(y, x)[1];
              unsigned char r = image.at<cv::Vec3b>(y, x)[2];

              cv::Vec3f & yuv = ret.at<cv::Vec3f>(y, x);
              yuv[0] =  0.299f*r + 0.587f*g + 0.114f*b;
              yuv[1] = -0.299f*r - 0.587f*g + 0.886f*b;
              yuv[2] =  0.701f*r - 0.587f*g - 0.114f*b;
          }
        }
      }

    } // end namespace
  }
}
