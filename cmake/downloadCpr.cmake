configure_and_build(cpr)

# CPR gtest might clash with gtest
set(BUILD_CPR_TESTS OFF CACHE BOOL "" FORCE)
# Add the dependency directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/cpr
        ${CMAKE_BINARY_DIR}/cpr
        EXCLUDE_FROM_ALL)

# The original cpr does not define include directories :(
target_include_directories(cpr SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/external/cpr/include)
