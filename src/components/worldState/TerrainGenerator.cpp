#include "TerrainGenerator.h"
#include <FastNoise/FastNoise.h>

namespace ape {
  namespace worldState {


    cv::Mat TerrainGenerator::getHeightMap(int width, int height) {
      FastNoise myNoise;
      myNoise.SetNoiseType(FastNoise::SimplexFractal);

      cv::Mat heightMap(height, width, CV_32F);

      for (int y = 0; y < height; y++)
      {
        for (int x = 0; x < width; x++)
        {
          heightMap.at<float>(y, x) = myNoise.GetNoise(x, y);
        }
      }
      return heightMap;
    }

    cv::Mat TerrainGenerator::getOgre3DModel(int width, int height) {
      cv::Mat heightMap = getHeightMap(width, height);
      return heightMap;
    }


    TerrainGenerator::TerrainGenerator() {

    }

    TerrainGenerator::~TerrainGenerator() {

    }
  }
}