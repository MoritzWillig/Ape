project(ApeImageProcessingComponent CPP)

set(APE_IMAGEPROCESSING_COMPONENT_SOURCES
        ${APE_IMAGEPROCESSING_SOURCES}
        imageProcessing.cpp
        )

add_library(apeImageProcessing OBJECT ${APE_IMAGEPROCESSING_COMPONENT_SOURCES})