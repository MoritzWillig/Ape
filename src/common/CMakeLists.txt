project(ApeCommon CXX)

set(APE_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#list header files for vs
file(GLOB_RECURSE HEADERS APE_COMMON_DIR "*.h")

add_library(apeCommon OBJECT
        ${HEADERS})
