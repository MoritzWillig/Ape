project(DemoScene CXX)

set(DEMO_SCENE_SOURCES
        demoScene.cpp
        )

add_executable(DemoScene
        ${DEMO_SCENE_SOURCES}
        $<TARGET_OBJECTS:apeVisualization>
        )

include_directories(${OGRE_INCLUDE_DIR})
target_link_libraries(DemoScene ${OGRE_LIBRARIES})