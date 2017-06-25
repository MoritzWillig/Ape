project(ApeDesktop CXX)

#get section sources
get_directory_property(
        APE_SECTION_APPSTATE_SOURCES DIRECTORY ../sections
        DEFINITION APE_SECTION_APPSTATE_SOURCES)

#sources for the application
set(APE_SOURCES
        ape.cpp
        )

#all required sources - including sections and components
add_executable(apeDesktop
        #application sources
        ${APE_SOURCES}
        #sections
        ${APE_SECTION_APPSTATE_SOURCES}
        #components
        ${APE_INTERFACE_SOURCES}
        $<TARGET_OBJECTS:apeImageProcessing>
        #$<TARGET_OBJECTS:textureGen>
        $<TARGET_OBJECTS:apeVisualization>
        #$<TARGET_OBJECTS:worldState>
        )

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${OGRE_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})

include_directories(${APE_COMPONENTS_INCLUDE_DIR})


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(apeDesktop Threads::Threads)

target_link_libraries(apeDesktop ${OpenCV_LIBRARIES})
target_link_libraries(apeDesktop ${OGRE_LIBRARIES})
include_directories(apeDesktop ${glfw3_LIBRARIES})
target_link_libraries(apeDesktop glfw)
