# Configure dependency from externalProject
configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/externalProjectGSL.txt.in
        GSL-download/CMakeLists.txt
)

# run cmake and build the downloaded source
cmake_generate_directory(GSL-download)
build_directory(GSL-download)

# Add the dependency directly to our build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/GSL
        ${CMAKE_BINARY_DIR}/GSL
        EXCLUDE_FROM_ALL)
