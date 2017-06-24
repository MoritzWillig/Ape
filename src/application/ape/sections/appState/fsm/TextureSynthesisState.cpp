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

            TextureSynthesis::TextureSynthesis(
                ape::imageProcessing::IImageProcessingController* ipController,
                ape::visualization::IVisualizationController* visController):
                ipController(ipController), visController(visController) {
            }

            void TextureSynthesis::onActivation() {
              visController->setOverlay(
                  ape::visualization::IVisualizationController::Overlay::TextureSynthesisSelection,
                  true
              );
            }

            void TextureSynthesis::onDeactivation() {
              visController->setOverlay(
                  ape::visualization::IVisualizationController::Overlay::TextureSynthesisSelection,
                  false
              );
            }

            void TextureSynthesis::update(float delta) {

            }

          }
        }
      }
    }
  }
}
