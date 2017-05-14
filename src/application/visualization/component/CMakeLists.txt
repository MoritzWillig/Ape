project(ApeVisualizationComponent CXX)

get_directory_property(
        APE_VISUALIZATION_INCLUDE_DIR DIRECTORY ../../../components/visualization
        DEFINITION APE_VISUALIZATION_INCLUDE_DIR DIRECTORY)

get_directory_property(
        APE_VISUALIZATION_SOURCES DIRECTORY ../../../components/visualization
        DEFINITION APE_VISUALIZATION_SOURCES)

include_directories(${APE_VISUALIZATION_INCLUDE_DIR})

#FIXME remove this should not be needed outside the component
include_directories(${OGRE_INCLUDE_DIR})

set(APE_VISUALIZATION_COMPONENT_SOURCES
        ${APE_VISUALIZATION_SOURCES}
        AppWindow.cpp
        visualization.cpp
        )

add_definitions(-DOGRE_PLUGIN_DIR=\"${OGRE_PLUGIN_DIR}\")

add_library(apeVisualization OBJECT ${APE_VISUALIZATION_COMPONENT_SOURCES})