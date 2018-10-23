# Download the latest range-v3 from Github
configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/externalProjectRange-v3.txt.in
        range-v3-download/CMakeLists.txt
)

# Configure and build the downloaded range-v3 source
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/range-v3-download)
if(result)
    message(FATAL_ERROR "CMake step for range-v3 failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/range-v3-download)
if(result)
    message(FATAL_ERROR "Build step for range-v3 failed: ${result}")
endif()

# Add range-v3 directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/range-v3
        ${CMAKE_BINARY_DIR}/range-v3
        EXCLUDE_FROM_ALL)

