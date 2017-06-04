#pragma once
//
// Created by moritz on 03.06.17.
//

#include "State.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {
          namespace fsm {

            class MainMenuState : public State {
            private:
            protected:
              virtual void onActivation() override;
              virtual void onDeactivation() override;
            public:
              //Default
              MainMenuState() = default;

              // Copy constructor
              MainMenuState(const MainMenuState&) = default;

              // Move constructor
              MainMenuState(MainMenuState&&) = default;

              // Copy assignment operator
              MainMenuState& operator=(const MainMenuState&)& = default;

              // Move assignment operator
              MainMenuState& operator=(MainMenuState&&)& = default;

              // Destructor
              virtual ~MainMenuState() = default;
            };

          }
        }
      }
    }
  }
}
