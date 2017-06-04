#pragma once
//
// Created by moritz on 03.06.17.
//

#include "common/signals/Signal.h"


namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {
          namespace fsm {

            //FIXME move to model
            class State {
            private:
              bool active;
            protected:

              virtual void onActivation() = 0;
              virtual void onDeactivation() = 0;
            public:
              //Default
              State();

              // Copy constructor
              State(const State&) = default;

              // Move constructor
              State(State&&) = default;

              // Copy assignment operator
              State& operator=(const State&)& = default;

              // Move assignment operator
              State& operator=(State&&)& = default;

              // Destructor
              virtual ~State() = default;

              void setActive(bool active);

              bool isActive();

              virtual void update(float delta) = 0;
            };

          }
        }
      }
    }
  }
}
