//
// Created by moritz on 24.06.17.
//

#include "ProcessingContextImplementation.h"

namespace ape {

  namespace imageProcessing {

    ProcessingContextImplementation::ProcessingContextImplementation():
      context(Context::Image), imageContext(), streamContext(nullptr) {
    }

    ProcessingContext::Context ProcessingContextImplementation::getContext() {
      return context;
    }

    void ProcessingContextImplementation::setStreamContext(
        ape::imageProcessing::CameraStream* stream) {
      context=Context::Stream;
      streamContext=stream;
      imageContext.release();
    }

    void ProcessingContextImplementation::setImageContext(cv::Mat mat) {
      context=Context::Image;
      imageContext=mat;
      streamContext=nullptr;
    }

    CameraStream* ProcessingContextImplementation::getStreamContext() {
      if (context!=Context::Stream) {
        throw std::runtime_error("The requested context is not active");
      }

      return streamContext;
    }

    cv::Mat ProcessingContextImplementation::getImageContext() {
      if (context!=Context::Stream) {
        throw std::runtime_error("The requested context is not active");
      }

      return imageContext;
    }

    cv::Mat ProcessingContextImplementation::getContextValue() {
      switch (context) {
        case Context::Stream:
          return streamContext->getCurrentFrame();
        case Context::Image:
          return imageContext;
      }
    }

  }
}