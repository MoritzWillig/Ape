#FIXME rename
set(APE_COMPONENTS_INCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        )

add_subdirectory(interfaces)
add_subdirectory(components)
add_subdirectory(application)
add_subdirectory(common)
