//
// Created by moritz on 03.06.17.
//

#include "WorldScreenState.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {
          namespace fsm {

            WorldScreenState::WorldScreenState(
                ape::imageProcessing::ImageProcessingController* ipController,
                ape::visualization::IVisualizationController* visController):
                ipController(ipController), visController(visController) {
            }

            void WorldScreenState::onActivation() {
              visController->setOverlay(
                  ape::visualization::IVisualizationController::Overlay::WorldScreen
              );
            }

            void WorldScreenState::onDeactivation() {

            }

            void WorldScreenState::update(float delta) {
              visController->setViewTransform(ipController->getTransformation());
            }

          }
        }
      }
    }
  }
}
