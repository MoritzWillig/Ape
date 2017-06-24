#pragma once
//
// Created by moritz on 03.06.17.
//

#include "State.h"
#include "../../../../visualization/component/visualization.h"
#include "../../../../imageProcessing/component/imageProcessing.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {
          namespace fsm {

            class WorldScreenState : public State {
            private:
              ape::imageProcessing::IImageProcessingController* ipController;
              ape::visualization::IVisualizationController* visController;
            protected:
              virtual void onActivation() override;
              virtual void onDeactivation() override;
            public:
              //Default
              WorldScreenState() = delete;

              WorldScreenState(
                  ape::imageProcessing::IImageProcessingController* ipController,
                  ape::visualization::IVisualizationController* visController
              );

              // Copy constructor
              WorldScreenState(const WorldScreenState&) = default;

              // Move constructor
              WorldScreenState(WorldScreenState&&) = default;

              // Copy assignment operator
              WorldScreenState& operator=(const WorldScreenState&)& = default;

              // Move assignment operator
              WorldScreenState& operator=(WorldScreenState&&)& = default;

              // Destructor
              virtual ~WorldScreenState() = default;

              void update(float delta) override;
            };

          }
        }
      }
    }
  }
}
