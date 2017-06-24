//
// Created by moritz on 05.06.17.
//

#include <glm/vec2.hpp>
#include <overlay/OgreShapes.h>
#include "LoadingControllerStage.h"


namespace ape {
  namespace visualization {

    LoadingControllerStage::LoadingControllerStage(AppWindow* appWindow):
        Stage(appWindow), stageLength{0.7f, 0.7f, 0.4f, 1.5f, INFINITY},
        percent(0.0f), stageProgress(0.0f), face() {

      /*auto p1=std::make_shared<shapes::OgrePolygon2D>(
          appWindow,std::vector<glm::vec2>({
                                               glm::vec2(+0.95, +0.7),
                                               glm::vec2(+0.95, +0.95),
                                               glm::vec2(+0.7, +0.95)}),
          Ogre::ColourValue(1.0f,0.6f,0.0f));
      face.childs.emplace_back(p1);*/

      //eye outer l
      auto p5=std::make_shared<shapes::OgrePolygon2D>(
          appWindow,std::vector<glm::vec2>({
                                               glm::vec2(-0.28-0.2, +0.23+0.00),
                                               glm::vec2(-0.28-0.09, +0.23+0.2),
                                               glm::vec2(-0.28+0.09, +0.23+0.2),
                                               glm::vec2(-0.28+0.2, +0.23+0.00),
                                               glm::vec2(-0.28+0.09, +0.23-0.2),
                                               glm::vec2(-0.28-0.09, +0.23-0.2)
                                           }),
          Ogre::ColourValue(1.0f,1.0f,1.0f));
      face.childs.emplace_back(p5);

      //inner top
      auto p4=std::make_shared<shapes::OgrePolygon2D>(
          appWindow,std::vector<glm::vec2>({
                                               glm::vec2(-0.50, +0.70),
                                               glm::vec2(+0.50, +0.70),
                                               glm::vec2(+0.20, +0.15),
                                               glm::vec2(-0.20, +0.15)
                                           }),
          Ogre::ColourValue(0.9f,0.6f,0.0f));
      face.childs.emplace_back(p4);

      //outline
      auto p2=std::make_shared<shapes::OgrePolygon2D>(
          appWindow,std::vector<glm::vec2>({
                                               glm::vec2(-0.40, -0.75),
                                               glm::vec2(+0.40, -0.75),
                                               glm::vec2(+0.80, -0.05),
                                               glm::vec2(+0.40, +0.65),
                                               glm::vec2(-0.40, +0.65),
                                               glm::vec2(-0.80, -0.05)
                                           }),
          Ogre::ColourValue(0.0f,0.0f,0.0f));
      face.childs.emplace_back(p2);

      //inner middle
      auto p3=std::make_shared<shapes::OgrePolygon2D>(
          appWindow,std::vector<glm::vec2>({
                                               glm::vec2(-0.00, -0.08),
                                               glm::vec2(-0.28, -0.08),
                                               glm::vec2(-0.30, -0.05),
                                               glm::vec2(-0.20, +0.15),
                                               glm::vec2(-0.20, +0.50),
                                               glm::vec2(-0.00, +0.65),
                                               glm::vec2(+0.20, +0.50),
                                               glm::vec2(+0.20, +0.15),
                                               glm::vec2(+0.30, -0.05),
                                               glm::vec2(+0.28, -0.08),
                                           }),
          Ogre::ColourValue(0.0f,0.6f,0.4f));
      face.childs.emplace_back(p3);

      setActive(false);
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