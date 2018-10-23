# Download the latest gflags from Github
configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/externalProjectGflags.txt.in
        gflags-download/CMakeLists.txt
)

# Configure and build the downloaded gflags source
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gflags-download )
if(result)
    message(FATAL_ERROR "CMake step for gflags failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gflags-download)
if(result)
    message(FATAL_ERROR "Build step for gflags failed: ${result}")
endif()

# Add gflags directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/gflags
        ${CMAKE_BINARY_DIR}/gflags
        EXCLUDE_FROM_ALL)
