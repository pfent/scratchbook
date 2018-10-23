# Download the latest RapidJSON from Github
configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/externalProjectRapidJSON.txt.in
        RapidJSON-download/CMakeLists.txt
)

# Configure and build the downloaded RapidJSON source
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/RapidJSON-download )
if(result)
    message(FATAL_ERROR "CMake step for RapidJSON failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/RapidJSON-download)
if(result)
    message(FATAL_ERROR "Build step for RapidJSON failed: ${result}")
endif()

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
