project(Ape CXX)

set(APE_SOURCES
        ape.cpp
        )

add_executable(ape
        ${APE_SOURCES}
        $<TARGET_OBJECTS:apeImageProcessing>
        #$<TARGET_OBJECTS:textureGen>
        #$<TARGET_OBJECTS:visulization>
        #$<TARGET_OBJECTS:worldState>
        )