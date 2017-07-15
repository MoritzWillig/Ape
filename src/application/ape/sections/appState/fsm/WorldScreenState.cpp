//
// Created by moritz on 03.06.17.
//

#include "WorldScreenState.h"
#include "../../../../worldState/component/worldState.h"

namespace ape {
  namespace app {
    namespace desktop {
      namespace section {
        namespace appState {
          namespace fsm {

            WorldScreenState::WorldScreenState(
                ape::worldState::IWorldStateController* wsController,
                ape::imageProcessing::IImageProcessingController* ipController,
                ape::visualization::IVisualizationController* visController):
                wsController(wsController), ipController(ipController),
                visController(visController), selectedEntity(-1) {

              visController->entitySelectionHandler.setCallback([](
                  void* custom,
                  ape::visualization::IVisualModel::VisualModelHandle visualModelHandle) -> void {
                auto self=(WorldScreenState*)custom;

                if (!self->isActive()) {
                  return;
                }

                std::cout<<"Selected entity handle: "<<visualModelHandle<<std::endl;
                self->selectedEntity=visualModelHandle;

                if (self->selectedEntity<0) {
                  return;
                }

                //begin surface selection
                self->visController->setOverlay(
                    ape::visualization::IVisualizationController::Overlay::SurfaceSelection,
                    true
                );

                //show surface selection -> update entity;
                self->visController->surfaceSelectionHandler.setCallback([](
                    void* custom,
                    ape::visualization::IVisualizationController::
                    SurfaceSelectionAction action,
                    std::string surfaceName
                ) -> void {
                  auto self=(WorldScreenState*)custom;

                  //TODO we should unregister the callback handler
                  //this only hides incorrect behaviour
                  if (self->selectedEntity<0) {
                    return;
                  }

                  switch (action) {
                    case visualization::IVisualizationController::SurfaceSelectionAction::SELECT_TEMPORARY:
                      //TODO currently ignored
                      break;
                    case visualization::IVisualizationController::SurfaceSelectionAction::SELECT_PERMANENT:
                      self->wsController->setSurface(
                          self->selectedEntity,
                          self->wsController->getSurfaceByName(surfaceName)
                          );

                      //selection finished, close overlay
                      self->visController->setOverlay(
                          ape::visualization::IVisualizationController::Overlay::SurfaceSelection,
                          false
                      );
                      break;
                    case visualization::IVisualizationController::SurfaceSelectionAction::RESET:
                      //TODO currently ignored
                      break;
                  }
                }, self);

              }, this);

            }

            void WorldScreenState::onActivation() {
              visController->setOverlay(
                  ape::visualization::IVisualizationController::Overlay::WorldScreen,
                  true
              );
            }

            void WorldScreenState::onDeactivation() {
              visController->setOverlay(
                  ape::visualization::IVisualizationController::Overlay::WorldScreen,
                  false
              );
              visController->surfaceSelectionHandler.setCallback(nullptr,
                                                                 nullptr);
              visController->setOverlay(
                  ape::visualization::IVisualizationController::Overlay::TextureSynthesisSelection,
                  false
              );
            }

            void WorldScreenState::update(float delta) {
              visController->setViewTransform(ipController->getTransformation());
            }

          }
        }
      }
    }
  }
}
