set(APE_COMPONENTS_INCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
        )

set(APE_INTERFACE_SOURCES
        ${APE_COMPONENTS_INCLUDE_DIR}/visualization/overlay/Shapes.cpp
        )

add_subdirectory(components)
add_subdirectory(application)
