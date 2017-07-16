//
// Created by moritz on 03.06.17.
//

#include "TextureSynthesisState.h"
#include "../../../../worldState/component/worldState.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {
          namespace fsm {

            TextureSynthesisState::TextureSynthesisState(
                ape::imageProcessing::IImageProcessingController* ipController,
                ape::visualization::IVisualizationController* visController,
                ape::worldState::IWorldStateController* wsController):
                ipController(ipController), visController(visController),
                wsController(wsController), counter() {
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

                textureGenerationFinishedHandler->callExceptIfNotSet(id);
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

            ape::worldState::ISurface::SurfacePersistentHandle
            TextureSynthesisState::generateTile(
                glm::vec2 vertex1, glm::vec2 vertex2) {
              auto min = glm::min(vertex1, vertex2);
              auto max = glm::max(vertex1, vertex2);

              //convert screen space into camera texture space
              auto cs=ipController->getCameraStream();
              min.x=(float)(min.x+1.0)*0.5f*cs->getFrameWidth();
              min.y=(float)(min.y+1.0)*0.5f*cs->getFrameHeight();
              max.x=(float)(max.x+1.0)*0.5f*cs->getFrameWidth();
              max.y=(float)(max.y+1.0)*0.5f*cs->getFrameHeight();

              cv::Rect roi((int)min.x,(int)min.y,(int)(max.x-min.x),(int)(max.y-min.y));
              auto region=ipController->extractTextureFromStream(roi);
              auto tile=ipController->createTile(512,512,region);

              //register at world state controller
              auto handle = wsController->createSurface(
                  "genSurface_"+std::to_string(counter.getNew()),tile);

              //update at visController?
              //visController->registerSurface("name...",tile);
              return handle;
            }

          }
        }
      }
    }
  }
}
