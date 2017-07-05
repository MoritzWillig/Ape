project(Color CXX)

set(COLOR_SOURCES
    color.cpp
    )

add_executable(Color
        ${COLOR_SOURCES}
        $<TARGET_OBJECTS:apeImageProcessing>
        )

include_directories(${APE_COMPONENTS_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(Color ${OpenCV_LIBRARIES})
