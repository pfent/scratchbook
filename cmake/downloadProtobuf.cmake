configure_and_build(protobuf)

# Protobuf's gmock might clash with googletest
set(protobuf_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# Add the dependency directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/protobuf/cmake
        ${CMAKE_BINARY_DIR}/protobuf
        EXCLUDE_FROM_ALL)
