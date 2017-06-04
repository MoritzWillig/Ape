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

            State::State():
                active(false) {
            }

            void State::setActive(bool active) {
              this->active=active;
              this->active?onActivation():onDeactivation();
            }

            bool State::isActive() {
              return active;
            }

          }
        }
      }
    }
  }
}