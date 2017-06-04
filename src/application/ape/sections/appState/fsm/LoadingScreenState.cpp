//
// Created by moritz on 03.06.17.
//

#include "LoadingScreenState.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {
          namespace fsm {

            LoadingScreenState::LoadingScreenState(
                ape::imageProcessing::ImageProcessingController ipController,
                ape::visualization::VisualizationController visController,
                Signal* onLoadingComplete):
                ipController(ipController), visController(visController),
                onLoadingComplete(onLoadingComplete) {

            }

            void LoadingScreenState::onActivation() {
              visController.setOverlay(
                  ape::visualization::VisualizationController::Overlay::Loading
              );
            }

            void LoadingScreenState::onDeactivation() {

            }

            void LoadingScreenState::update(float delta) {
              //if (isLoadingComplete) {
                onLoadingComplete->set(true);
              //}
            }

          }
        }
      }
    }
  }
}
