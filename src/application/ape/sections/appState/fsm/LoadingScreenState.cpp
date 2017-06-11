//
// Created by moritz on 03.06.17.
//

#include <iostream>
#include "LoadingScreenState.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {
          namespace fsm {

            LoadingScreenState::LoadingScreenState(
                ape::imageProcessing::ImageProcessingController* ipController,
                ape::visualization::IVisualizationController* visController,
                Signal* onLoadingComplete):
                ipController(ipController), visController(visController),
                onLoadingComplete(onLoadingComplete),
                animationProgress(0.0), loadingProgress(0.0) {

            }

            bool LoadingScreenState::isLoadingCompleted() {
              return (
                  (animationProgress>=animationTime) && (loadingProgress>=1.0)
              );
            }

            void LoadingScreenState::onActivation() {
              animationProgress=0.0;
              loadingProgress=0.0;

              visController->setOverlay(
                  ape::visualization::IVisualizationController::Overlay::Loading
              );
            }

            void LoadingScreenState::onDeactivation() {

            }

            void LoadingScreenState::update(float delta) {
              if (isLoadingCompleted()) {
                onLoadingComplete->set(true);
              } else {
                loadingProgress=1.0;
                animationProgress+=delta;
                visController->setProgress(loadingProgress);
              }
            }

          }
        }
      }
    }
  }
}
