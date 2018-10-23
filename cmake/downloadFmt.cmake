# Download the latest fmt from Github
configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/externalProjectFmt.txt.in
        fmt-download/CMakeLists.txt
)

# Configure and build the downloaded fmt source
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/fmt-download )
if(result)
    message(FATAL_ERROR "CMake step for fmt failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/fmt-download)
if(result)
    message(FATAL_ERROR "Build step for fmt failed: ${result}")
endif()

# Add fmt directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/fmt
        ${CMAKE_BINARY_DIR}/fmt
        EXCLUDE_FROM_ALL)
