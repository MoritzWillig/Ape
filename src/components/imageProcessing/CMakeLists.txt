set(APE_IMAGEPROCESSING_COMPONENT_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

set(APE_IMAGEPROCESSING_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenCVCameraStream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/FileCameraStream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LazyCameraStream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/textureSynthesis/TextureExtraction.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/textureSynthesis/TextureSynthesis.cpp
        )
