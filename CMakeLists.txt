cmake_minimum_required(VERSION 3.1.0)

#set cmake flags
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV 3 REQUIRED)

add_subdirectory(src)