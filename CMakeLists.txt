cmake_minimum_required(VERSION 3.1.0)

#set cmake flags
set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    set(prefix "")
    set(suffix ".lib")
elseif(APPLE)
    set(prefix "lib")
    set(suffix ".a")
else()
    set(prefix "lib")
    set(suffix ".a")
endif()

set(OPEN_LIBRARIES
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/opencv/customInstall/lib/${prefix}opencv_core${suffix}"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/opencv/customInstall/lib/${prefix}opencv_videoio${suffix}")
set(OPENCV_INCLUDE_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/opencv/customInstall/include")

add_subdirectory(src)