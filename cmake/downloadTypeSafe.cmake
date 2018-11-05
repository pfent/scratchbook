configure_and_build(type_safe)

# Add the dependency directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/type_safe
        ${CMAKE_BINARY_DIR}/type_safe
        EXCLUDE_FROM_ALL)
