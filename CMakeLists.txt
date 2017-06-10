cmake_minimum_required(VERSION 3.1.0)

#set cmake flags
set(CMAKE_CXX_STANDARD 11)

# move executables to specific folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release")

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
            "${OpenCVInstDir}/${prefix}opencv_videoio${suffix}"
            "${OpenCVInstDir}/${prefix}opencv_calib3d${suffix}"
            "${OpenCVInstDir}/${prefix}opencv_imgproc${suffix}")
    set(OpenCV_INCLUDE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/opencv/customInstall/include")
endif()


if(WIN32)
    set(OGRE_LIBRARIES
            "${Boost_LIBRARIES}"
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/ogre_src_v1-8-1/customInstall/lib/release/${prefix}OgreMain${suffix}")
    set(OGRE_INCLUDE_DIR
            "${Boost_INCLUDE_DIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/ogre_src_v1-8-1/customInstall/include")
    set(OGRE_PLUGIN_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/ogre_src_v1-8-1/customInstall/bin/release/")
else()
    set(OGRE_LIBRARIES
            "${Boost_LIBRARIES}"
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/ogre_src_v1-8-1/customInstall/lib/${prefix}OgreMain${suffix}")
    set(OGRE_INCLUDE_DIR
            "${Boost_INCLUDE_DIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/ogre_src_v1-8-1/customInstall/include")
    set(OGRE_PLUGIN_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/ogre_src_v1-8-1/customInstall/lib/OGRE")
endif()

add_subdirectory(src)
