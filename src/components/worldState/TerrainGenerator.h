#pragma once
#include "opencv2\opencv.hpp"


namespace ape {
  namespace worldState {

    class TerrainGenerator {
    private:

    protected:
    public:

      cv::Mat getHeightMap(int width, int height);
      cv::Mat getOgre3DModel(int width, int height);

      //Default
      TerrainGenerator();

      // Copy constructor
      TerrainGenerator(const TerrainGenerator&) = default;

      // Move constructor
      TerrainGenerator(TerrainGenerator&&) = default;

      // Copy assignment operator
      TerrainGenerator& operator=(const TerrainGenerator&)& = default;

      // Move assignment operator
      TerrainGenerator& operator=(TerrainGenerator&&)& = default;

      // Destructor
      virtual ~TerrainGenerator();

    };

  }
}
