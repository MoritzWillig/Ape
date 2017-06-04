//
// Created by moritz on 03.06.17.
//

#include "AppStateController.h"


namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {

          AppStateController::AppStateController(
              ape::imageProcessing::ImageProcessingController* ipController,
              ape::visualization::IVisualizationController* visController):
              onLoadingCompleteSignal(this),
              lsState(ipController,visController, &onLoadingCompleteSignal),
              mmState(ipController,visController),
              wsState(ipController,visController),
              activeState(lsState), requestedState(lsState) {
            lsState.setActive(true);
          };

          void AppStateController::transition(fsm::State& newState) {
            activeState.setActive(false);
            activeState=newState;
            activeState.setActive(true);
          }

          void AppStateController::update(float delta) {
            if (&requestedState!=&activeState) {
              transition(requestedState);
            }
          }

          void AppStateController::requestTransition(State newState) {
            switch (newState) {
              case State::LoadingScreen:
                requestedState=lsState;
                break;
              case State::MainMenu:
                requestedState=mmState;
                break;
              case State::WorldScreen:
                requestedState=wsState;
                break;
            }
          }

        }
      }
    }
  }
}