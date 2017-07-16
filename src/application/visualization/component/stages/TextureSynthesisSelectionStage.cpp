//
// Created by moritz on 05.06.17.
//

#include <glm/gtx/string_cast.hpp>
#include <overlay/OgreButton.h>
#include "TextureSynthesisSelectionStage.h"


namespace ape {
  namespace visualization {

    TextureSynthesisSelectionStage::TextureSynthesisSelectionStage(
        AppWindow* appWindow,
        CustomValueCallback<
            IVisualizationController::OverlayChangeRequestHandler,
            void*>& overlayChangeRequestHandler,
        CustomValueCallback<
            IVisualizationController::TextureGenerationRequestHandler,
            void*>& textureGenerationRequestHandler):
        Stage(appWindow), overlay(), abortButton(),
        overlayChangeRequestHandler(overlayChangeRequestHandler),
        textureGenerationRequestHandler(textureGenerationRequestHandler),
        vertex1(), vertex2(), lastMousePosition(), generationFinishedHandler(),
        selectionState(SelectionState::CapturingP1) {

      abortButton=std::make_shared<ape::visualization::shapes::OgreButton>(
          appWindow,
          0.79,
          0.99,
          0.2,
          0.2,
          Ogre::ColourValue(1.0,1.0,1.0,1.0f)
      );
      abortButton->textureName.setValue("close_ico");
      abortButton->updateOgreObject();

      overlay.childs.push_back(abortButton);
      setActive(false);
    }

    void TextureSynthesisSelectionStage::setActive(bool active) {
      this->active=active;

      if (this->active) {
        vertex1.reset();
        vertex2.reset();
        selectionState = SelectionState::CapturingP1;
        performStateTransition(SelectionState::CapturingP1);

        overlay.setVisible(true);
      } else {
        overlay.setVisible(false);
      }
    }

    void TextureSynthesisSelectionStage::update(float delta) {
      if (!active) {
        return;
      }
    }

    void TextureSynthesisSelectionStage::performStateTransition(
        TextureSynthesisSelectionStage::SelectionState newSelectionState) {
      //disable old state elements
      switch (selectionState) {
        case SelectionState::CapturingP1:break;
        case SelectionState::CapturingP2:break;
        case SelectionState::Waiting:break;
        case SelectionState::Generating:break;
        case SelectionState::Confirmation:break;
      }
      selectionState=newSelectionState;
      //add new state elements
      switch (selectionState) {
        case SelectionState::CapturingP1:
          std::cout<<"Click to select first edge"<<std::endl;
          break;
        case SelectionState::CapturingP2:
          std::cout<<"Click to select second edge"<<std::endl;
          //FIXME display currently selected area
          break;
        case SelectionState::Waiting:
          std::cout<<"Press ENTER to proceed"<<std::endl;
          std::cout<<"This will generate a texture from "
                 <<glm::to_string(vertex1.getValue())<<" | "
                 <<glm::to_string(vertex2.getValue())<< std::endl;
          //FIXME add currently selected area
          //FIXME add edit handlers
          break;
        case SelectionState::Generating: {
          std::cout<<"Generating"<<std::endl;

          generationFinishedHandler.setCallback([](void* custom, int textureId) {
            auto self=(TextureSynthesisSelectionStage*)custom;

            std::cout<<"recived generated texture "<<textureId<<std::endl;

            self->performStateTransition(SelectionState::Confirmation);
          },this);

          textureGenerationRequestHandler.callExceptIfNotSet(
              vertex1.getValue(),
              vertex2.getValue(),
              &generationFinishedHandler
          );

          //FIXME show loading bar ...
        }
          break;
        case SelectionState::Confirmation:
          std::cout<<"Generation successful. Press any key to proceed"<<std::endl;
          //FIXME show result - "name" field + "ok"/"_abort" button
          break;
      }
    }

    void TextureSynthesisSelectionStage::processKeyEvent(
        int key, int scancode, int action, int mods) {
      Stage::processKeyEvent(key, scancode, action, mods);

      if (!active) {
        return;
      }

      bool canBeCanceled=true;
      switch (selectionState) {
        case SelectionState::CapturingP1:break;
        case SelectionState::CapturingP2:break;
        case SelectionState::Waiting:
          //wait for user to confirm the selected region
          if (key==GLFW_KEY_ENTER && action==GLFW_RELEASE) {
            performStateTransition(SelectionState::Generating);
          }
          break;
        case SelectionState::Generating:
          canBeCanceled=false;
          break;
        case SelectionState::Confirmation:
          //proceed with any key
          if (action==GLFW_RELEASE) {
            overlayChangeRequestHandler.callExceptIfNotSet(
                IVisualizationController::Overlay::WorldScreen);
          }
          break;
      }

      switch (key) {
        case GLFW_KEY_ESCAPE:
          if (canBeCanceled && action==GLFW_RELEASE) {
            overlayChangeRequestHandler.callExceptIfNotSet(
                IVisualizationController::Overlay::WorldScreen);
          }
          break;
        default:
          break;
      }
    }

    void TextureSynthesisSelectionStage::processMousePositionEvent(double x,
                                                                   double y) {
      Stage::processMousePositionEvent(x, y);

      if (!active) {
        return;
      }

      lastMousePosition.x=(float)x;
      lastMousePosition.y=(float)y;
    }

    void TextureSynthesisSelectionStage::processMouseButtonEvent(int button,
                                                                 int action,
                                                                 int mods,
                                                                 bool& cancel) {
      Stage::processMouseButtonEvent(button, action, mods, cancel);

      if (!active) {
        return;
      }

      if ((button==GLFW_MOUSE_BUTTON_LEFT) && (action==GLFW_RELEASE)) {
        if ((abortButton->isVisible()) && abortButton->hit(lastMousePosition)) {
          overlayChangeRequestHandler.callExceptIfNotSet(
              IVisualizationController::Overlay::WorldScreen);
          return;
        }
      }

      switch (selectionState) {
        case SelectionState::CapturingP1:
          if ((button==GLFW_MOUSE_BUTTON_LEFT) && (action==GLFW_RELEASE)) {
            vertex1.setValue(lastMousePosition);
            performStateTransition(SelectionState::CapturingP2);
          }
          break;
        case SelectionState::CapturingP2:
          if ((button==GLFW_MOUSE_BUTTON_LEFT) && (action==GLFW_RELEASE)) {
            vertex2.setValue(lastMousePosition);
            performStateTransition(SelectionState::Waiting);
          }
          break;
        case SelectionState::Waiting:
          //wait for the user to edit the vectors (not implemented yet)
          //or to press enter
          break;
        case SelectionState::Generating:
          //this stage can not be canceled by user input ...
          break;
        case SelectionState::Confirmation:
          //FIXME not implemented
          break;
      }
    }

  }
}