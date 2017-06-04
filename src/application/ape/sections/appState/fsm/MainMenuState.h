#pragma once
//
// Created by moritz on 03.06.17.
//

#include "State.h"
#include "../../../../imageProcessing/component/imageProcessing.h"
#include "../../../../visualization/component/visualization.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {
          namespace fsm {

            class MainMenuState : public State {
            private:
              ape::imageProcessing::ImageProcessingController ipController;
              ape::visualization::VisualizationController visController;
            protected:
              virtual void onActivation() override;
              virtual void onDeactivation() override;
            public:
              //Default
              MainMenuState() = delete;

              MainMenuState(
                  ape::imageProcessing::ImageProcessingController ipController,
                  ape::visualization::VisualizationController visController
              );

              // Copy constructor
              MainMenuState(const MainMenuState&) = default;

              // Move constructor
              MainMenuState(MainMenuState&&) = default;

              // Copy assignment operator
              MainMenuState& operator=(const MainMenuState&)& = default;

              // Move assignment operator
              MainMenuState& operator=(MainMenuState&&)& = default;

              // Destructor
              virtual ~MainMenuState() = default;

              void update(float delta) override;
            };

          }
        }
      }
    }
  }
}
