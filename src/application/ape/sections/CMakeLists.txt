
set(APE_SECTION_APPSTATE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/appState)

set(APE_SECTION_APPSTATE_SOURCES
        ${APE_SECTION_APPSTATE_DIR}/fsm/State.cpp
        ${APE_SECTION_APPSTATE_DIR}/fsm/LoadingScreenState.cpp
        ${APE_SECTION_APPSTATE_DIR}/fsm/MainMenuState.cpp
        ${APE_SECTION_APPSTATE_DIR}/fsm/WorldScreenState.cpp

        ${APE_SECTION_APPSTATE_DIR}/AppStateController.cpp
        )