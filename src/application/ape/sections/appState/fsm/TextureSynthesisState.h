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

            class TextureSynthesis : public State {
            private:
              ape::imageProcessing::ImageProcessingController* ipController;
              ape::visualization::IVisualizationController* visController;
            protected:
              virtual void onActivation() override;
              virtual void onDeactivation() override;
            public:
              //Default
              TextureSynthesis() = delete;

              TextureSynthesis(
                  ape::imageProcessing::ImageProcessingController* ipController,
                  ape::visualization::IVisualizationController* visController
              );

              // Copy constructor
              TextureSynthesis(const TextureSynthesis&) = default;

              // Move constructor
              TextureSynthesis(TextureSynthesis&&) = default;

              // Copy assignment operator
              TextureSynthesis& operator=(const TextureSynthesis&)& = default;

              // Move assignment operator
              TextureSynthesis& operator=(TextureSynthesis&&)& = default;

              // Destructor
              virtual ~TextureSynthesis() = default;

              void update(float delta) override;
            };

          }
        }
      }
    }
  }
}
