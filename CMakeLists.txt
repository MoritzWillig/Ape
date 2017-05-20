cmake_minimum_required(VERSION 3.1.0)

#set cmake flags
set(CMAKE_CXX_STANDARD 11)

find_package(Boost 1.4.9 REQUIRED
        COMPONENTS thread date_time)

if(WIN32)
    set(prefix "")
    set(suffix ".lib")
elseif(APPLE)
    set(prefix "lib")
    set(suffix ".a")
else()
    set(prefix "lib")
    set(suffix ".so")
endif()

set(GLM_INCLUDE_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/glm/include")

find_package(OpenCV 3.2.0 QUIET)
if (OpenCV_FOUND)
    #found a standard installation
else()
    #use the custom install
    set(OpenCVInstDir "${CMAKE_CURRENT_SOURCE_DIR}/libs/opencv/customInstall/lib")
    set(OpenCV_LIBRARIES
            "${OpenCVInstDir}/${prefix}opencv_core${suffix}"
            "${OpenCVInstDir}/${prefix}opencv_aruco${suffix}"
            "${OpenCVInstDir}/${prefix}opencv_highgui${suffix}"
            "${OpenCVInstDir}/${prefix}opencv_videoio${suffix}")
    set(OpenCV_INCLUDE_DIR
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/opencv/customInstall/include")
endif()

set(OGRE_LIBRARIES
        "${Boost_LIBRARIES}"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/ogre_src_v1-8-1/customInstall/lib/${prefix}OgreMain${suffix}")
set(OGRE_INCLUDE_DIR
        "${Boost_INCLUDE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/ogre_src_v1-8-1/customInstall/include")
set(OGRE_PLUGIN_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/ogre_src_v1-8-1/customInstall/lib/OGRE")

#set(BGFX_LIBRARIES
#        "${CMAKE_CURRENT_SOURCE_DIR}/libs/bgfx/.build/${prefix}opencv_core${suffix}"
#        "${CMAKE_CURRENT_SOURCE_DIR}/libs/bgfx/.build/${prefix}opencv_videoio${suffix}")
#set(BGFX_INCLUDE_DIR
#        "${CMAKE_CURRENT_SOURCE_DIR}/libs/bgfx/include"
#        "${CMAKE_CURRENT_SOURCE_DIR}/libs/bimg/include"
#        "${CMAKE_CURRENT_SOURCE_DIR}/libs/bx/include")

add_subdirectory(src)
