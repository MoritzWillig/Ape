cmake_minimum_required(VERSION 3.1.0)

#set cmake flags
set(CMAKE_CXX_STANDARD 11)


find_package(Boost 1.4.9 REQUIRED
        COMPONENTS thread date_time)
find_package(OpenCV REQUIRED)

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

# set(OpenCV_LIBRARIES
#         "${CMAKE_CURRENT_SOURCE_DIR}/libs/opencv/customInstall/lib/${prefix}opencv_core${suffix}"
#         "${CMAKE_CURRENT_SOURCE_DIR}/libs/opencv/customInstall/lib/${prefix}opencv_videoio${suffix}")
# set(OpenCV_INCLUDE_DIR
#         "${CMAKE_CURRENT_SOURCE_DIR}/libs/opencv/customInstall/include")

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
