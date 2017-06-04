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

            class LoadingScreenState : public State {
            private:
            protected:
              virtual void onActivation() override;
              virtual void onDeactivation() override;
            public:
              //Default
              LoadingScreenState() = default;

              // Copy constructor
              LoadingScreenState(const LoadingScreenState&) = default;

              // Move constructor
              LoadingScreenState(LoadingScreenState&&) = default;

              // Copy assignment operator
              LoadingScreenState&
              operator=(const LoadingScreenState&)& = default;

              // Move assignment operator
              LoadingScreenState& operator=(LoadingScreenState&&)& = default;

              // Destructor
              virtual ~LoadingScreenState() = default;
            };

          }
        }
      }
    }
  }
}
