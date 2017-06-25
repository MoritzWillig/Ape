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

            class TextureSynthesisState : public State {
            private:
              ape::imageProcessing::IImageProcessingController* ipController;
              ape::visualization::IVisualizationController* visController;
            protected:
              virtual void onActivation() override;
              virtual void onDeactivation() override;

              void generateTile(glm::vec2 vertex1, glm::vec2 vertex2);
            public:
              //Default
              TextureSynthesisState() = delete;

              TextureSynthesisState(
                  ape::imageProcessing::IImageProcessingController* ipController,
                  ape::visualization::IVisualizationController* visController
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
