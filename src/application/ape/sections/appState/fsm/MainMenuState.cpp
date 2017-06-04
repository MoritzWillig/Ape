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
                ape::imageProcessing::ImageProcessingController ipController,
                ape::visualization::VisualizationController visController):
                ipController(ipController), visController(visController) {

            }

            void MainMenuState::onActivation() {
              visController.setOverlay(
                  ape::visualization::VisualizationController::Overlay::Menu
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
