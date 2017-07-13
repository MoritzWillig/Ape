#pragma once
//
// Created by moritz on 03.06.17.
//

#include "fsm/State.h"
#include "fsm/LoadingScreenState.h"
#include "fsm/MainMenuState.h"
#include "fsm/WorldScreenState.h"
#include "fsm/TextureSynthesisState.h"
#include "../../../imageProcessing/component/imageProcessing.h"
#include "../../../visualization/component/visualization.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {

          class AppStateController {
          private:
            class OnLoadingCompleteSignal : public Signal {
            private:
              AppStateController* asController;
            public:
              OnLoadingCompleteSignal(AppStateController* asController):
                  asController(asController) {
              }

              void set(bool signal) {
                Signal::set(signal);
                asController->requestTransition(State::WorldScreen);
              }
            } onLoadingCompleteSignal;

          protected:
            fsm::LoadingScreenState lsState;
            fsm::MainMenuState mmState;
            fsm::WorldScreenState wsState;
            fsm::TextureSynthesisState tsState;

            fsm::State* activeState;
            fsm::State* requestedState;

            void transition(fsm::State* newState);
          public:
            //Default
            AppStateController()=delete;

            AppStateController(
                ape::worldState::IWorldStateController* wsController,
                ape::imageProcessing::IImageProcessingController* ipController,
                ape::visualization::IVisualizationController* visController);

            // Copy constructor
            AppStateController(const AppStateController&) = default;

            // Move constructor
            AppStateController(AppStateController&&) = default;

            // Copy assignment operator
            AppStateController& operator=(const AppStateController&)& = default;

            // Move assignment operator
            AppStateController& operator=(AppStateController&&)& = default;

            // Destructor
            virtual ~AppStateController() = default;

            void update(float delta);

            enum class State {
              LoadingScreen,
              MainMenu,
              WorldScreen,
              TextureSynthesisSelection
            };

            void requestTransition(State newState);

          };

        }
      }
    }
  }
}
