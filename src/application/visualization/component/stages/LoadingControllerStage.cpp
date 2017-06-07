//
// Created by moritz on 05.06.17.
//

#include <glm/vec2.hpp>
#include "LoadingControllerStage.h"


namespace ape {
  namespace visualization {

    LoadingControllerStage::LoadingControllerStage():
        active(false), stageLength{0.7f, 0.7f, 0.4f, 1.5f, INFINITY},
        percent(0.0f), stageProgress(0.0f), face() {

      //FIXME static array
      /*shapes::Polygon2D stat(3, {
          glm::vec2(-0.3, -1.0),
          glm::vec2(+0.3, -1.0),
          glm::vec2(+0.0, +0.0)
      });*/

      //FIXME ...

    }

    void LoadingControllerStage::setActive(bool active) {
      this->active=active;
      face.setVisible(active);

      if (active) {
        percent=0.0;
        stageProgress=0.0;
        animationStage=AnimationStage::MidUp;
      }
    }

    void LoadingControllerStage::update(float progress) {
      if (!active) {
        return;
      }

      auto idx=getCurrentStageIndex();
      if (stageProgress+progress>stageLength[idx]) {
        //finish current stage
        auto pprogress=stageLength[idx]-stageProgress;
        update(pprogress);

        //proceed to next stage
        switch (animationStage) {
          case AnimationStage::MidUp:
            animationStage=AnimationStage::TDown;
            break;
          case AnimationStage::TDown:
            animationStage=AnimationStage::RLIn;
            break;
          case AnimationStage::RLIn:
            animationStage=AnimationStage::OGrow;
            break;
          case AnimationStage::OGrow:
            animationStage=AnimationStage::Finished;
            break;
        }
        stageProgress=0.0;

        //start next stage
        update(progress-pprogress);
      } else {
        percent+=progress;
        stageProgress+=progress;

        //FIXME warten auf Animation class ...
        switch (animationStage) {
          case AnimationStage::MidUp:
            //TODO
            break;
          case AnimationStage::TDown:
            //TODO
            break;
          case AnimationStage::RLIn:
            //TODO
            break;
          case AnimationStage::OGrow:
            //TODO
            break;
        }
      }
    }

    unsigned int LoadingControllerStage::getCurrentStageIndex() {
      switch (animationStage) {
        case AnimationStage::MidUp:
          return 0;
          break;
        case AnimationStage::TDown:
          return 1;
          break;
        case AnimationStage::RLIn:
          return 2;
          break;
        case AnimationStage::OGrow:
          return 3;
          break;
        case AnimationStage::Finished:
          return 4;
          break;
      }
    }

  }
}