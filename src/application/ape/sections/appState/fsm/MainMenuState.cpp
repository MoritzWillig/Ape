//
// Created by moritz on 03.06.17.
//

#include "MainMenuState.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {
          namespace fsm {

            MainMenuState::MainMenuState(
                ape::imageProcessing::ImageProcessingController* ipController,
                ape::visualization::IVisualizationController* visController):
                ipController(ipController), visController(visController) {

            }

            void MainMenuState::onActivation() {
              visController->setOverlay(
                  ape::visualization::IVisualizationController::Overlay::Menu
              );
            }

            void MainMenuState::onDeactivation() {

            }

            void MainMenuState::update(float delta) {

            }

          }
        }
      }
    }
  }
}
