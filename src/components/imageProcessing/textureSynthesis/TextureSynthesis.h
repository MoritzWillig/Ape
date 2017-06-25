// adapted from: http://deepeco.ucsd.edu/~hye/cs176/project1.html
#pragma once

#include "opencv2/videoio.hpp"

namespace ape {
  namespace imageProcessing {

    class TextureSynthesis {
    private:
      void initializeTexture(int size);
      void findBestMatch(int i, int j, int size);

      int sample_w, sample_h;
      unsigned char** sample_red;
      unsigned char** sample_green;
      unsigned char** sample_blue;

      int texture_w, texture_h;
      unsigned char** texture_red;
      unsigned char** texture_green;
      unsigned char** texture_blue;
      int** original_pos_x;
      int** original_pos_y;

      unsigned char** red;
      unsigned char** green;
      unsigned char** blue;
      std::vector<int> candidate_x;
      std::vector<int> candidate_y;

    protected:
    public:
      TextureSynthesis(cv::Mat const& image, std::size_t texture_w,
        std::size_t texture_h);

      // Copy constructor
      TextureSynthesis(const TextureSynthesis&) = default;

      // Move constructor
      TextureSynthesis(TextureSynthesis&&) = default;

      // Copy assignment operator
      TextureSynthesis& operator=(const TextureSynthesis&)& = default;

      // Move assignment operator
      TextureSynthesis& operator=(TextureSynthesis&&)& = default;

      // Destructor
      ~TextureSynthesis() = default;

      void generateTexture(int windowSize, cv::Mat &texture);
    };

  }
}


