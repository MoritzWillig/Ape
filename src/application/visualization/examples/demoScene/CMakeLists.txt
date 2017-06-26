project(DemoScene CXX)

set(DEMO_SCENE_SOURCES
        demoScene.cpp
        )

add_executable(DemoScene
        ${DEMO_SCENE_SOURCES}
        ${APE_INTERFACE_SOURCES}
        $<TARGET_OBJECTS:apeVisualization>
        $<TARGET_OBJECTS:apeWorldState>
        )

include_directories(${OGRE_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})

include_directories(${APE_COMPONENTS_INCLUDE_DIR})
add_definitions(-DOGRE_PLUGIN_DIR=\"${OGRE_PLUGIN_DIR}\")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(DemoScene Threads::Threads)

target_link_libraries(DemoScene ${OGRE_LIBRARIES})
