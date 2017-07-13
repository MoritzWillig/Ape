project(ApeInterfaces CXX)

set(APE_INTERFACES_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#list header files for vs
file(GLOB_RECURSE HEADERS APE_INTERFACES_DIR "*.h")

include_directories(${GLM_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

set(APE_INTERFACE_SOURCES
        ${APE_INTERFACES_DIR}/common/generators/Counter.cpp
        )

add_library(apeInterfaces OBJECT
        ${APE_INTERFACE_SOURCES}
        ${HEADERS})
