#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "image_tools.h"

namespace ape {
  namespace imageProcessing {

    void equalizeHistogram(cv::Mat & image) {
      cv::cvtColor(image, image, CV_BGR2YCrCb);
      cv::Mat channels[3];
      cv::split(image, channels);

      cv::equalizeHist(channels[0], channels[0]);
      cv::merge(channels, 3, image);

      cv::cvtColor(image, image, CV_YCrCb2BGR);
    }

//#define DEBUG_GREY_PIXELS
#define CH_B 0
#define CH_G 1
#define CH_R 2
// double step threshold
#define THRES_A 0.8f
// convergence threshold
#define THRES_B 0.15f
#define AUTOWB_MAX_ITERS 100

    void robustAWB(cv::Mat & image, float T, float gainIncrement)
    {
      // Define helper functions sign(x) and K(x) as described in paper
      auto sign = [] (float x) -> float {
        if (x > 0.0f) return 1.0f;
        else if (x < 0.0f) return -1.0f;
        else return 0.0f;
      };

      auto K = [&] (float x) -> float {
          if (std::abs(x) >= THRES_A)
              return 2.0f * sign(x);
          else if (std::abs(x) < THRES_B)
              return 0.0f;
          else
            return 0.5f * sign(x);
      };

      // gains for blue, green and red channels
      cv::Vec3f gains(1.0f, 1.0f, 1.0f);
      cv::Mat orig = image.clone();

      for (int j = 0; j < AUTOWB_MAX_ITERS; ++j)
      {
          // apply gain
          for (int y = 0; y < image.rows; ++y) {
            for (int x = 0; x < image.cols; ++x) {
              cv::Vec3b & bgr = image.at<cv::Vec3b>(y, x);
              cv::Vec3b const& bgrOrig = orig.at<cv::Vec3b>(y, x);
              for (int ch = 0; ch < 3; ++ch) {
                bgr[ch] = std::max((unsigned char)0,
                  std::min((unsigned char)255,
                  (unsigned char) (bgrOrig[ch] * gains[ch])));
              }
            }
          }

//#define DEBUG_GREY_PIXELS
#ifdef DEBUG_GREY_PIXELS
        cv::Mat grays(image.rows, image.cols, CV_8UC3);
#endif
        // convert image to float
        cv::Mat imageFloat;
        image.convertTo(imageFloat, CV_32FC3);

        // convert to YUV
        cv::Mat yuvImage(image.rows, image.cols, CV_32FC3);
        for (int y = 0; y < image.rows; ++y) {
          for (int x = 0; x < image.cols; ++x) {

              unsigned char b = image.at<cv::Vec3b>(y, x)[0];
              unsigned char g = image.at<cv::Vec3b>(y, x)[1];
              unsigned char r = image.at<cv::Vec3b>(y, x)[2];
              cv::Vec3f & yuv = yuvImage.at<cv::Vec3f>(y, x);

              yuv[0] =  0.299f*r + 0.587f*g + 0.114f*b;
              yuv[1] = -0.299f*r - 0.587f*g + 0.886f*b;
              yuv[2] =  0.701f*r - 0.587f*g - 0.114f*b;
          }
        }

        // estimate illumination: find grey points in the image which satisfy
        // (|U| + |V|) / Y < T
        // where T is some threshold
        float avgU = 0.0f;
        float avgV = 0.0f;
        std::size_t count = 0;
        for (int y = 0; y < yuvImage.rows; ++y)
        {
          for (int x = 0; x < yuvImage.cols; ++x)
          {
            float Y = yuvImage.at<cv::Vec3f>(y, x)[0];
            float U = yuvImage.at<cv::Vec3f>(y, x)[1];
            float V = yuvImage.at<cv::Vec3f>(y, x)[2];

            float val = (std::abs(U) + std::abs(V)) / Y;
            if (val < T) {
              avgU += U;
              avgV += V;
              count++;
#ifdef DEBUG_GREY_PIXELS
              grays.at<cv::Vec3b>(y, x) = cv::Vec3b(0,0,255);
#endif
            }
          }
        }

        if (count == 0) {
            std::cout << "No gray pixels found in image!" << std::endl;
            return;
        }
        avgU /= (float) count;
        avgV /= (float) count;

#ifdef DEBUG_GREY_PIXELS
        std::stringstream fn;
        fn << "/tmp/grays_" << j << ".png";
        cv::imwrite(fn.str(), grays);
#endif

        float phi;
        std::size_t c;
        if ((std::abs(avgU) > std::abs(avgV))
          || (std::abs(avgU) == std::abs(avgV) && std::abs(avgU) != 0.0f))
        {
          // color is biased towards blue --> adjust gain of blue channel
          c = CH_B;
          phi = avgU;
        }
        else if (std::abs(avgU) < std::abs(avgV)) {
          // color is biased towards red --> adjust gain of red channel
          c = CH_R;
          phi = avgV;
        }
        else {
            // avgU == avgV == 0
            std::cout << "Converged after " << j << " iterations." << std::endl;
            return;
        }


        // err = d - phi, but d is 0, so it's just -phi (see equ. 14)
        float KVal = K(-phi);
        if (KVal == 0.0f) {
            std::cout << "Converged after " << j << " iterations." << std::endl;
            return;
        }
        gains[c] = gains[c] + gainIncrement * KVal;
        std::cout << "Iter: " << j
          << " u_avg: " << avgU << " v_avg: " << avgV
          << " gains: " << gains << std::endl;
      } // end iteration
    }


  } // namespace end
} // namespace end
