project(ApeImageProcessingComponent CXX)

get_directory_property(
        APE_IMAGEPROCESSING_INCLUDE_DIR DIRECTORY ../../../components/imageProcessing
        DEFINITION APE_IMAGEPROCESSING_INCLUDE_DIR DIRECTORY)

get_directory_property(
        APE_IMAGEPROCESSING_SOURCES DIRECTORY ../../../components/imageProcessing
        DEFINITION APE_IMAGEPROCESSING_SOURCES)

include_directories(${APE_IMAGEPROCESSING_INCLUDE_DIR})

set(APE_IMAGEPROCESSING_COMPONENT_SOURCES
        ${APE_IMAGEPROCESSING_SOURCES}
        imageProcessing.cpp
        )

add_library(apeImageProcessing OBJECT ${APE_IMAGEPROCESSING_COMPONENT_SOURCES})