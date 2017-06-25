#pragma once

#include "imageProcessing/ProcessingContext.h"

namespace ape {
  namespace imageProcessing {

    class ProcessingContextImplementation : public ProcessingContext {
    private:
    protected:
      Context context;

      cv::Mat imageContext;
      CameraStream* streamContext;
    public:
      //Default
      ProcessingContextImplementation();

      // Copy constructor
      ProcessingContextImplementation(
          const ProcessingContextImplementation&) = default;

      // Move constructor
      ProcessingContextImplementation(
          ProcessingContextImplementation&&) = default;

      // Copy assignment operator
      ProcessingContextImplementation&
      operator=(const ProcessingContextImplementation&)& = default;

      // Move assignment operator
      ProcessingContextImplementation&
      operator=(ProcessingContextImplementation&&)& = default;

      // Destructor
      virtual ~ProcessingContextImplementation() = default;

      virtual Context getContext() override;

      virtual void setStreamContext(CameraStream* stream) override;

      virtual void setImageContext(cv::Mat mat) override;

      CameraStream* getStreamContext();

      cv::Mat getImageContext();

      cv::Mat getContextValue();
    };

  }
}

