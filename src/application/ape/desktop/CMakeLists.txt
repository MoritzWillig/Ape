project(ApeDesktop CXX)

set(APE_SOURCES
        ape.cpp
        )

add_executable(apeDesktop
        ${APE_SOURCES}
        $<TARGET_OBJECTS:apeImageProcessing>
        #$<TARGET_OBJECTS:textureGen>
        #$<TARGET_OBJECTS:visulization>
        #$<TARGET_OBJECTS:worldState>
        )

include_directories(${OPENCV_INCLUDE_DIR})
target_link_libraries(apeDesktop ${OPENCV_LIBRARIES})