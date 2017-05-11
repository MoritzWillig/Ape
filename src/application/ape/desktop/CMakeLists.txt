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

target_link_libraries(apeDesktop ${OpenCV_LIBS})