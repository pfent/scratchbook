configure_and_build(GSL)

# Add the dependency directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/GSL
        ${CMAKE_BINARY_DIR}/GSL
        EXCLUDE_FROM_ALL)
