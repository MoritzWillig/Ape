#pragma once
//
// Created by moritz on 03.06.17.
//

#include "fsm/State.h"
#include "fsm/LoadingScreenState.h"
#include "fsm/MainMenuState.h"
#include "fsm/WorldScreenState.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {

          class AppStateController {
          private:
          protected:
            fsm::LoadingScreenState lsState;
            fsm::MainMenuState mmState;
            fsm::WorldScreenState wsState;

            fsm::State& activeState;
            fsm::State& requestedState;

            void transition(fsm::State& newState);
          public:
            //Default
            AppStateController();

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
              WorldScreen
            };

            void requestTransition(State newState);

          };

        }
      }
    }
  }
}
