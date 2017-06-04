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

            class WorldScreenState : public State {
            private:
            protected:
              virtual void onActivation() override;
              virtual void onDeactivation() override;
            public:
              //Default
              WorldScreenState() = default;

              // Copy constructor
              WorldScreenState(const WorldScreenState&) = default;

              // Move constructor
              WorldScreenState(WorldScreenState&&) = default;

              // Copy assignment operator
              WorldScreenState& operator=(const WorldScreenState&)& = default;

              // Move assignment operator
              WorldScreenState& operator=(WorldScreenState&&)& = default;

              // Destructor
              virtual ~WorldScreenState() = default;
            };

          }
        }
      }
    }
  }
}
