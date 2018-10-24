configure_and_build(gflags)

# Add gflags directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/gflags
        ${CMAKE_BINARY_DIR}/gflags
        EXCLUDE_FROM_ALL)
