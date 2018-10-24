configure_and_build(fmt)

# Add fmt directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/fmt
        ${CMAKE_BINARY_DIR}/fmt
        EXCLUDE_FROM_ALL)
