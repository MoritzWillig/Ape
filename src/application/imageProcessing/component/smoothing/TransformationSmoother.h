#pragma once

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <tuple>
#include <common/signals/Signal.h>
#include <glm/gtc/quaternion.hpp>

#include "ISmoother.h"
#include "MeanSmoother.h"

namespace ape {
  namespace imageProcessing {

    template<typename TranslationType, typename RotationType>
    class ViewParameters {
    private:
    protected:
    public:
      ViewParameters():
          translation(), rotation() {
      }

      ViewParameters(
          TranslationType translation,
          RotationType rotation):
          translation(translation),
          rotation(rotation) {
      }

      TranslationType translation;
      RotationType rotation;
    };

    template <
        typename TranslationType,
        typename RotationType,
        typename TranslationSmoother,
        typename RotationSmoother>
    class TransformationSmoother: public ISmoother<
        ViewParameters<TranslationType,RotationType>,
        ViewParameters<TranslationType,RotationType>> {
    private:
    protected:
      TranslationSmoother translationSmoother;
      RotationSmoother rotationSmoother;
    public:
      //Default
      TransformationSmoother() = default;

      TransformationSmoother(
          TranslationSmoother&& translationSmoother,
        RotationSmoother&& rotationSmoother
      ):
          translationSmoother(translationSmoother),
          rotationSmoother(rotationSmoother) {
      }

      // Copy constructor
      TransformationSmoother(const TransformationSmoother&) = default;

      // Move constructor
      TransformationSmoother(TransformationSmoother&&) = default;

      // Copy assignment operator
      TransformationSmoother&
      operator=(const TransformationSmoother&)& = default;

      // Move assignment operator
      TransformationSmoother& operator=(TransformationSmoother&&)& = default;

      // Destructor
      virtual ~TransformationSmoother() = default;

      virtual void recordValue(ViewParameters<TranslationType,RotationType> value) override {
        translationSmoother.recordValue(value.translation);
        rotationSmoother.recordValue(value.rotation);
      }

      virtual ViewParameters<TranslationType,RotationType> getSmoothedValue() const override {
        ViewParameters<TranslationType,RotationType> params;
        params.translation=translationSmoother.getSmoothedValue();
        params.rotation=rotationSmoother.getSmoothedValue();
        return params;
      }

      virtual void clearHistory() override {
        translationSmoother.clearHistory();
        rotationSmoother.clearHistory();
      }
    };

  }
}
