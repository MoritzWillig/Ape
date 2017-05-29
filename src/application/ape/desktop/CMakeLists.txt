project(ApeDesktop CXX)

set(APE_SOURCES
        ape.cpp
        )

add_executable(apeDesktop
        ${APE_SOURCES}
        $<TARGET_OBJECTS:apeImageProcessing>
        #$<TARGET_OBJECTS:textureGen>
        $<TARGET_OBJECTS:apeVisualization>
        #$<TARGET_OBJECTS:worldState>
        )

include_directories(${OpenCV_INCLUDE_DIR})
include_directories(${OGRE_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})

#FIXME needed?
include_directories(${APE_COMPONENTS_INCLUDE_DIR})

target_link_libraries(apeDesktop ${OpenCV_LIBRARIES})
target_link_libraries(apeDesktop ${OGRE_LIBRARIES})
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
