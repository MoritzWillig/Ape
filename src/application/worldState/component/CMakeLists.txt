project(ApeWorldStateComponent CXX)

get_directory_property(
        APE_WORLDSTATE_COMPONENT_DIR DIRECTORY ../../../components/worldState
        DEFINITION APE_WORLDSTATE_COMPONENT_DIR)

get_directory_property(
        APE_WORLDSTATE_SOURCES DIRECTORY ../../../components/worldState
        DEFINITION APE_WORLDSTATE_SOURCES)

#add interface headersQ
include_directories(${APE_COMPONENTS_INCLUDE_DIR})

#add component specific classes
include_directories(${APE_WORLDSTATE_COMPONENT_DIR})

#FIXME this is needed by the model. A variable containing this path
#should be defined there
include_directories(${OGRE_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${FASTNOISE_INCLUDE_DIR})

#merge model and controller to component target
set(APE_WORLDSTATE_COMPONENT_SOURCES
        ${APE_WORLDSTATE_SOURCES}
        worldState.cpp
        worldFileBasedState.cpp
        SurfaceImplementation.cpp
        )

add_library(apeWorldState OBJECT ${APE_WORLDSTATE_COMPONENT_SOURCES})
