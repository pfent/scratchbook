configure_and_build(RapidJSON)

# RapidJSON tests clash with gtest / gmock
set(RAPIDJSON_BUILD_TESTS OFF)
set(RAPIDJSON_BUILD_THIRDPARTY_GTEST OFF)

# Add RapidJSON directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/RapidJSON
        ${CMAKE_BINARY_DIR}/RapidJSON
        EXCLUDE_FROM_ALL)
find_package(RapidJSON)

# The original RapidJSON does not define include directories :(
add_library(RapidJSON INTERFACE)
target_include_directories(RapidJSON SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/external/RapidJSON/include)
