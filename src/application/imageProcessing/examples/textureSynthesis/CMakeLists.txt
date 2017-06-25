project(TextureSynthesis CXX)

set(TEXTURE_SYNTHESIS_SOURCES
        textureSynthesis.cpp
        )

add_executable(TextureSynthesis
        ${TEXTURE_SYNTHESIS_SOURCES}
        $<TARGET_OBJECTS:apeImageProcessing>
        )

include_directories(${APE_COMPONENTS_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(TextureSynthesis ${OpenCV_LIBRARIES})
