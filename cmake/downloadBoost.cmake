# Download the latest Boost from Github
configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/externalProjectBoost.txt.in
        Boost-download/CMakeLists.txt
)

# Configure and build the downloaded Boost source
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Boost-download )
if(result)
    message(FATAL_ERROR "CMake step for Boost failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Boost-download)
if(result)
    message(FATAL_ERROR "Build step for Boost failed: ${result}")
endif()

# Add Boost directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/Boost
        ${CMAKE_BINARY_DIR}/Boost
        EXCLUDE_FROM_ALL)
