#pragma once
//
// Created by moritz on 20.05.17.
//

#include "imageProcessing/CameraStream.h"

#include "glm/glm.hpp"
#include <opencv2/aruco/dictionary.hpp>
#include <opencv2/aruco.hpp>

namespace ape {
  namespace imageProcessing {

    class Signal {
    private:
      bool signal;
    protected:
    public:
      Signal():
          signal(true) {
      }

      Signal(bool signal):
          signal(signal) {
      }

      void set() {
        signal=true;
      }

      void set(bool signal) {
        this->signal=signal;
      }

      void reset() {
        signal=false;
      }

      bool hasSignal() const {
        return signal;
      }

      explicit operator bool() const {
        return hasSignal();
      }
    };

    template <typename T>
    class SignalOrDefault {
    private:
      T defaultValue;

      const Signal* signal;
      T value;
    protected:
    public:
      SignalOrDefault(const Signal* signal, T defaultValue):
          defaultValue(defaultValue), signal(signal), value() {
      }

      SignalOrDefault(Signal* signal):
          defaultValue(), signal(signal), value() {
      }

      void setValue(T value) {
        this->value=value;
      }

      T get() const {
        return (*signal)?value:defaultValue;
      }

      bool hasSignal() {
        return signal->hasSignal();
      }

      explicit operator T() const {
        return get();
      }
    };

    class ImageProcessingController {
    private:
      //FIXME we dont want to show OpenCV- / LazyCameraStream to the public
      //but it would be nice to state the actual types here
      //(without using pointers)
      CameraStream* cvCameraStream;
      CameraStream* lazyCameraStream;

      Signal searchedMarkerSignal;
      SignalOrDefault<bool> marker;
      SignalOrDefault<glm::mat4x4> transformation;

      void updateTransformation();

      glm::mat3x3 cameraIntrinsics;
      float* distCoeffs;

      //aruco
      cv::Ptr<cv::aruco::Dictionary> dictionary;
      float markerLength;
      std::vector<int> ids;
      std::vector<std::vector< cv::Point2f > > corners, rejected;
      std::vector<cv::Vec3d> rvecs, tvecs;
      cv::Ptr<cv::aruco::DetectorParameters> detectorParams;
      glm::mat4x4 viewMatrix;
    protected:
    public:
      //Default
      ImageProcessingController() = delete;

      ImageProcessingController(
          glm::mat3x3 cameraIntrinsics,
          float* distCoeffs);

      // Copy constructor
      ImageProcessingController(const ImageProcessingController&) = default;

      // Move constructor
      ImageProcessingController(ImageProcessingController&&) = default;

      // Copy assignment operator
      ImageProcessingController&
      operator=(const ImageProcessingController&)& = default;

      // Move assignment operator
      ImageProcessingController&
      operator=(ImageProcessingController&&)& = default;

      // Destructor
      virtual ~ImageProcessingController() = default;

      void update(float timeDelta);

      CameraStream* getCameraStream();

      bool getTerminateRequest();

      bool hasMarker();

      glm::mat4x4 getTransformation();

    };

  }
}

