#pragma once
//
// Created by moritz on 05.06.17.
//


#include <cmath>
#include <visualization/overlay/Shapes.h>
#include <OGRE/OgrePolygon.h>

namespace ape {
  namespace visualization {

    class LoadingControllerStage {
    private:
      bool active;

      enum class AnimationStage {
        MidUp,
        TDown,
        RLIn,
        OGrow,
        Finished
      } animationStage;

      //FIXME static
      float stageLength[5];

      float percent;
      float stageProgress;

      shapes::Container face;
    protected:
    public:
      //Default
      LoadingControllerStage();

      // Copy constructor
      LoadingControllerStage(const LoadingControllerStage&) = default;

      // Move constructor
      LoadingControllerStage(LoadingControllerStage&&) = default;

      // Copy assignment operator
      LoadingControllerStage&
      operator=(const LoadingControllerStage&)& = default;

      // Move assignment operator
      LoadingControllerStage&
      operator=(LoadingControllerStage&&)& = default;

      // Destructor
      virtual ~LoadingControllerStage() = default;

      void setActive(bool active);

      void update(float progress);

      unsigned int getCurrentStageIndex();
    };

  }
}

