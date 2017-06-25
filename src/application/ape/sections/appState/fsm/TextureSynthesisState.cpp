//
// Created by moritz on 03.06.17.
//

#include "TextureSynthesisState.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {
          namespace fsm {

            TextureSynthesisState::TextureSynthesisState(
                ape::imageProcessing::IImageProcessingController* ipController,
                ape::visualization::IVisualizationController* visController):
                ipController(ipController), visController(visController) {
            }

            void TextureSynthesisState::onActivation() {
              visController->setOverlay(
                  ape::visualization::IVisualizationController::Overlay::TextureSynthesisSelection,
                  true
              );
            }

            void TextureSynthesisState::onDeactivation() {
              visController->setOverlay(
                  ape::visualization::IVisualizationController::Overlay::TextureSynthesisSelection,
                  false
              );
            }

            void TextureSynthesisState::update(float delta) {

            }

          }
        }
      }
    }
  }
}
