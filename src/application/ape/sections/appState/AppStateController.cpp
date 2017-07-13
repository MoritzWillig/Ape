//
// Created by moritz on 03.06.17.
//

#include <iostream>
#include "AppStateController.h"


namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {

          AppStateController::AppStateController(
              ape::worldState::IWorldStateController* wsController,
              ape::imageProcessing::IImageProcessingController* ipController,
              ape::visualization::IVisualizationController* visController):
              onLoadingCompleteSignal(this),
              lsState(ipController,visController, &onLoadingCompleteSignal),
              mmState(ipController,visController),
              wsState(wsController,ipController,visController),
              tsState(ipController,visController),
              activeState(&lsState), requestedState(&lsState) {
            lsState.setActive(true);
          };

          void AppStateController::transition(fsm::State* newState) {
            activeState->setActive(false);
            activeState=newState;
            activeState->setActive(true);
          }

          void AppStateController::update(float delta) {
            if (requestedState!=activeState) {
              transition(requestedState);
            }

            activeState->update(delta);
          }

          void AppStateController::requestTransition(State newState) {
            switch (newState) {
              case State::LoadingScreen:
                requestedState=&lsState;
                break;
              case State::MainMenu:
                requestedState=&mmState;
                break;
              case State::WorldScreen:
                requestedState=&wsState;
                break;
              case State::TextureSynthesisSelection:
                requestedState=&tsState;
                break;
            }
          }

        }
      }
    }
  }
}