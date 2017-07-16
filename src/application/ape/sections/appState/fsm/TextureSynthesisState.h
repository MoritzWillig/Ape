#pragma once
//
// Created by moritz on 03.06.17.
//

#include <common/generators/Counter.h>
#include "State.h"
#include "../../../../imageProcessing/component/imageProcessing.h"
#include "../../../../visualization/component/visualization.h"
#include "../../../../worldState/component/worldState.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {
          namespace fsm {

            class TextureSynthesisState : public State {
            private:
              ape::imageProcessing::IImageProcessingController* ipController;
              ape::visualization::IVisualizationController* visController;
              ape::worldState::IWorldStateController* wsController;

              Counter counter;
            protected:
              virtual void onActivation() override;
              virtual void onDeactivation() override;

              ape::worldState::ISurface::SurfacePersistentHandle
                generateTile(glm::vec2 vertex1, glm::vec2 vertex2);
            public:
              //Default
              TextureSynthesisState() = delete;

              TextureSynthesisState(
                  ape::imageProcessing::IImageProcessingController* ipController,
                  ape::visualization::IVisualizationController* visController,
                  ape::worldState::IWorldStateController* wsController
              );

              // Copy constructor
              TextureSynthesisState(const TextureSynthesisState&) = default;

              // Move constructor
              TextureSynthesisState(TextureSynthesisState&&) = default;

              // Copy assignment operator
              TextureSynthesisState& operator=(const TextureSynthesisState&)& = default;

              // Move assignment operator
              TextureSynthesisState& operator=(TextureSynthesisState&&)& = default;

              // Destructor
              virtual ~TextureSynthesisState() = default;

              void update(float delta) override;
            };

          }
        }
      }
    }
  }
}
