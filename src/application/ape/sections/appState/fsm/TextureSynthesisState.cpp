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

              visController->textureGenerationRequestHandler.setCallback([](
                  void* tsState, glm::vec2 vertex1, glm::vec2 vertex2,
                  CustomValueCallback<
                      ape::visualization::IVisualizationController::TextureGenerationFinishedHandler,
                      void*>*
                  textureGenerationFinishedHandler
              ) {
                auto self=(TextureSynthesisState*)tsState;
                auto id=self->generateTile(vertex1, vertex2);
                std::cout<<"Generated texture!"<<std::endl;
                //FIXME save image

                textureGenerationFinishedHandler->call(id);
              },this);
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

            unsigned int TextureSynthesisState::generateTile(
                glm::vec2 vertex1, glm::vec2 vertex2) {
              auto min = glm::min(vertex1, vertex2);
              auto max = glm::max(vertex1, vertex2);
              cv::Rect roi((int)min.x,(int)min.y,(int)(max.x-min.x),(int)(max.y-min.y));
              auto region=ipController->extractTextureFromStream(roi);
              auto tile=ipController->createTile(512,512,region);

              //FIXME pass actual texture id
              return 12345;
            }

          }
        }
      }
    }
  }
}
