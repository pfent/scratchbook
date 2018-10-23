# Download the latest GSL from Github
configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/externalProjectGSL.txt.in
        GSL-download/CMakeLists.txt
)

# Configure and build the downloaded GSL source
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/GSL-download )
if(result)
    message(FATAL_ERROR "CMake step for GSL failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/GSL-download)
if(result)
    message(FATAL_ERROR "Build step for GSL failed: ${result}")
endif()

# Add GSL directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/GSL
        ${CMAKE_BINARY_DIR}/GSL
        EXCLUDE_FROM_ALL)
