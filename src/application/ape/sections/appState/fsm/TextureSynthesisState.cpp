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

              //freeze stream to select image
              ipController->freezeCameraStream(true);

            }

            void TextureSynthesisState::onDeactivation() {
              //unfreeze stream
              ipController->freezeCameraStream(false);

              visController->setOverlay(
                  ape::visualization::IVisualizationController::Overlay::TextureSynthesisSelection,
                  false
              );
            }

            void TextureSynthesisState::update(float delta) {
            }

            void TextureSynthesisState::generateTile(
                glm::vec2 vertex1, glm::vec2 vertex2) {
              auto min = glm::min(vertex1, vertex2);
              auto max = glm::max(vertex1, vertex2);

              //FIXME constants ...
              auto width = 640;
              auto height = 480;
              auto extracted = ipController->extractTextureFromStream(
                  cv::Rect(
                      (int) (min.x * width),
                      (int) (min.y * height),
                      (int) ((max.x - min.x) * width),
                      (int) ((max.y - min.y) * height)
                  ));

              //FIXME make async!
              auto tile = ipController->createTile(
                  1000, 1000,
                  extracted
              );
              //FIXME save image

              //on finish transition visController to "Confirmation"
            }

          }
        }
      }
    }
  }
}
