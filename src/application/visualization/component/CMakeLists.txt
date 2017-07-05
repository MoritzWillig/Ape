project(ApeVisualizationComponent CXX)

get_directory_property(
        APE_VISUALIZATION_COMPONENT_DIR DIRECTORY ../../../components/visualization
        DEFINITION APE_VISUALIZATION_COMPONENT_DIR)

get_directory_property(
        APE_VISUALIZATION_SOURCES DIRECTORY ../../../components/visualization
        DEFINITION APE_VISUALIZATION_SOURCES)

#add interface headers
include_directories(${APE_COMPONENTS_INCLUDE_DIR})

#add component specific classes
include_directories(${APE_VISUALIZATION_COMPONENT_DIR})

#list header files for vs
file(GLOB_RECURSE HEADER1 "*.h")
file(GLOB_RECURSE HEADER2 APE_VISUALIZATION_COMPONENT_DIR "*.h")
file(GLOB_RECURSE HEADER3 APE_VISUALIZATION_SOURCES "*.h")

#FIXME this is needed by the model. A variable containing this path
#should be defined there
include_directories(${OGRE_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${glfw3_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

#merge model and controller to component target
set(APE_VISUALIZATION_COMPONENT_SOURCES
        ${APE_VISUALIZATION_SOURCES}
        visualization.cpp
        visualizationOGRE.cpp
        stages/Stage.cpp
        stages/LoadingControllerStage.cpp
        stages/TextureSynthesisSelectionStage.cpp
        stages/WorldScreenStage.cpp
        )

#FIXME this is needed by the model. A variable should be defined there
add_definitions(-DOGRE_PLUGIN_DIR=\"${OGRE_PLUGIN_DIR}\")

add_library(apeVisualization OBJECT ${APE_VISUALIZATION_COMPONENT_SOURCES} ${HEADER1} ${HEADER2} ${HEADER3})
