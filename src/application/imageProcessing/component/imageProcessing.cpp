#include <lzma.h>
#include "imageProcessing.h"


namespace ape {
  namespace imageProcessing {

    ImageProcessorController::ImageProcessorController() {

    }

    ImageProcessor* ImageProcessorController::getImageProcessor() {
      return nullptr;
    }
  }
}