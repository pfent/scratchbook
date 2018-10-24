configure_and_build(Boost)

# Add Boost directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/Boost
        ${CMAKE_BINARY_DIR}/Boost
        EXCLUDE_FROM_ALL)
