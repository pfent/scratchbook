configure_and_build(range-v3)

# Add range-v3 directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/range-v3
        ${CMAKE_BINARY_DIR}/range-v3
        EXCLUDE_FROM_ALL)

