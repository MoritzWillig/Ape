#pragma once
//
// Created by moritz on 05.06.17.
//


#include <cmath>
#include <overlay/Shapes.h>
#include <OGRE/OgrePolygon.h>
#include <AppWindow.h>

#include "Stage.h"

namespace ape {
  namespace visualization {

    class LoadingControllerStage: public Stage {
    private:
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
      LoadingControllerStage() = delete;

      LoadingControllerStage(AppWindow* appWindow);

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

