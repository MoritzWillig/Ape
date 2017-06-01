project(ApeImageProcessingComponent CXX)

get_directory_property(
        APE_IMAGEPROCESSING_COMPONENT_DIR DIRECTORY ../../../components/imageProcessing
        DEFINITION APE_IMAGEPROCESSING_COMPONENT_DIR)


get_directory_property(
        APE_IMAGEPROCESSING_SOURCES DIRECTORY ../../../components/imageProcessing
        DEFINITION APE_IMAGEPROCESSING_SOURCES)

#add interface headers
include_directories(${APE_COMPONENTS_INCLUDE_DIR})

#add component specific classes
include_directories(${APE_IMAGEPROCESSING_COMPONENT_DIR})

#add library deps
#FIXME this should be imported through a variable set by the model
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIR})

#merge model and controller to component target
set(APE_IMAGEPROCESSING_COMPONENT_SOURCES
        ${APE_IMAGEPROCESSING_SOURCES}
        imageProcessing.cpp
        )

add_library(apeImageProcessing OBJECT ${APE_IMAGEPROCESSING_COMPONENT_SOURCES})