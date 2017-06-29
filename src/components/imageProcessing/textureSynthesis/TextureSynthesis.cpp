// adapted from: http://deepeco.ucsd.edu/~hye/cs176/project1.html

#include <iostream>
#include <stdexcept>
#include <time.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"

#include "TextureSynthesis.h"

namespace ape {
  namespace imageProcessing {

    TextureSynthesis::TextureSynthesis(cv::Mat const& image,
      std::size_t texture_w, std::size_t texture_h)
      : texture_w(texture_w), texture_h(texture_h)
    {
      texture_red = new unsigned char* [texture_w];
      texture_green = new unsigned char* [texture_w];
      texture_blue = new unsigned char* [texture_w];
      original_pos_x = new int* [texture_w];
      original_pos_y = new int* [texture_w];

      for(int i = 0; i < texture_w; i++)
      {
        texture_red[i] = new unsigned char[texture_h];
        texture_green[i] = new unsigned char[texture_h];
        texture_blue[i] = new unsigned char[texture_h];
        original_pos_x[i] = new int[texture_w];
        original_pos_y[i] = new int[texture_w];
      }

      // read pixels
      sample_w = image.cols;
      sample_h = image.rows;

      // convert sample to individual channels arrays
      sample_red = new unsigned char*[sample_w];
      sample_green = new unsigned char*[sample_w];
      sample_blue = new unsigned char*[sample_w];

      for(std::size_t i = 0; i < sample_w; i++)
      {
        sample_red[i] = new unsigned char[sample_h];
        sample_green[i] = new unsigned char[sample_h];
        sample_blue[i] = new unsigned char[sample_h];
      }

      for (std::size_t j = 0; j < sample_h; j++)
      {
        for(std::size_t i = 0; i < sample_w; i++)
        {
          cv::Vec3b const& bgr = image.at<cv::Vec3b>(j, i);
          sample_blue[i][j] = bgr.val[0];
          sample_green[i][j] = bgr.val[1];
          sample_red[i][j] = bgr.val[2];
        }
      }
    }

    // generate the texture from the sample using a search window of size x size
    void TextureSynthesis::generateTexture(int windowSize, cv::Mat& texture)
    {

      std::cout<<"Initializing texture..." << std::endl;
      initializeTexture(windowSize);

      red = new unsigned char*[windowSize];
      green = new unsigned char*[windowSize];
      blue = new unsigned char*[windowSize];
      for(int x=0; x<windowSize; x++)
      {
        red[x] = new unsigned char[windowSize/2+1];
        blue[x] = new unsigned char[windowSize/2+1];
        green[x] = new unsigned char[windowSize/2+1];
      }

      int progress = 0;
      for(int j = 0; j < texture_h; j++)
      {
        if(j >= double(progress*texture_h)/100)
          std::cout<< (progress+=10) << "% done" << std::endl;

        for(int i = 0; i < texture_w; i++)
          findBestMatch(i, j, windowSize);
      }
      std::cout<<"100% done" << std::endl;
      std::cout<<"Texture generation complete" << std::endl;

      // write image in BGR format
      for (std::size_t j = 0; j < texture_h; j++)
      {
        for(std::size_t i = 0; i < texture_w; i++)
        {
          cv::Vec3b& bgr = texture.at<cv::Vec3b>(j, i);
          bgr[0] = texture_blue[i][j];
          bgr[1] = texture_green[i][j];
          bgr[2] = texture_red[i][j];
        }
      }
    }

    void TextureSynthesis::initializeTexture(int size)
    {
      int valid_w_length = sample_w-size+1;
      int valid_h_length = sample_h-size/2;
      int dw = size/2;
      int dh = size/2;

      srand(time(NULL));

      for (std::size_t j = 0; j < texture_h; j++)
      {
        for(std::size_t i = 0; i < texture_w; i++)
        {
          int w = rand() % valid_w_length + dw;
          int h = rand() % valid_h_length + dh;
          texture_red[i][j] = sample_red[w][h];
          texture_green[i][j] = sample_green[w][h];
          texture_blue[i][j] = sample_blue[w][h];
          original_pos_x[i][j] = w;
          original_pos_y[i][j] = h;
        }
      }
    }

    void TextureSynthesis::findBestMatch(int i, int j, int size)
    {
      // get candidates
      std::vector<int> candidate_x;
      std::vector<int> candidate_y;
      candidate_x.reserve(sample_w);
      candidate_y.reserve(sample_h);

      for(int actualh = j-size/2; actualh <= j; actualh++)
      {
        for(int actualw = i-size/2; actualw <= i+size/2; actualw++)
        {
          if(actualw==i && actualh==j)
          {
            actualh = j+1;
            break;
          }
          // make sure that x and y are within the bounds of the texture array
          int x = (actualw+texture_w)%texture_w;
          int y = (actualh+texture_h)%texture_h;

          // get the coordinates for the pixel in the sample image from which
          // the texture pixel came from and shift appropriately
          int actualx = original_pos_x[x][y]+i-actualw;
          int actualy = original_pos_y[x][y]+j-actualh;

          // check if neighborhood of candidate lies completely in sample
          if(actualx < size/2 || actualx >= sample_w-size/2
            || actualy < size/2 || actualy >= sample_h-size/2)
          {
            //replace with random
            actualx = rand()%(sample_w-size+1)+size/2;
            actualy = rand()%(sample_h-size/2)+size/2;
          }

          // add candidate if it hasn't been seen before
          bool insert = std::find(candidate_x.begin(), candidate_x.end(),
            actualx) == candidate_x.end();
          insert &= std::find(candidate_y.begin(), candidate_y.end(),
            actualy) == candidate_y.end();

          if (insert) {
            candidate_x.push_back(actualx);
            candidate_y.push_back(actualy);
          }
        }
      }

      // make local texture window
      for(int y=0, tj = j-size/2; y < size/2+1; y++, tj++)
      {
        for(int x=0, ti = i-size/2; x < size; x++, ti++)
        {
          // break if center of window reached (end of causal neighborhood)
          if(y==size/2 && x==size/2)
            break;

          // wrap around coordinates in texture image
          ti = (ti + texture_w) % texture_w;
          tj = (tj + texture_h) % texture_h;

          red[x][y] = texture_red[ti][tj];
          green[x][y] = texture_green[ti][tj];
          blue[x][y] = texture_blue[ti][tj];
        }
      }

      // find best match using candidates
      int bestd = std::numeric_limits<int>::max();
      int bestw = -1;
      int besth = -1;
      for(int c = 0; c < candidate_x.size(); c++)
      {
        int tempd = 0;
        for(int y=candidate_y[c]-size/2, tj=0; y<candidate_y[c]+1; y++, tj++)
        {
          for(int x=candidate_x[c]-size/2, ti=0; x<candidate_x[c]+size/2+1; x++, ti++)
          {
            if(y==candidate_y[c] && x==candidate_x[c] || tempd > bestd)
            {
              y = candidate_y[c]+1;
              break;
            }
            int r = int(red[ti][tj]-sample_red[x][y]);
            int g = int(green[ti][tj]-sample_green[x][y]);
            int b = int(blue[ti][tj]-sample_blue[x][y]);
            tempd += r*r + g*g + b*b;
          }
        }

        if(tempd < bestd)
        {
          bestw = candidate_x[c];
          besth = candidate_y[c];
          bestd = tempd;
        }
      }

      if (bestw < 0 || bestw > sample_w || besth < 0 || besth > sample_h) {
        throw std::runtime_error("Error finding match for position ("
          + std::to_string(i)+","+std::to_string(j)+"). Pixel lookup is out "
          + "of sample image range: ("+std::to_string(bestw)+","
          + std::to_string(besth)+")");
      }

      texture_red[i][j] = sample_red[bestw][besth];
      texture_green[i][j] = sample_green[bestw][besth];
      texture_blue[i][j] = sample_blue[bestw][besth];
      original_pos_x[i][j] = bestw;
      original_pos_y[i][j] = besth;
      return;
    }
  } // end namespace
} // end namespace
