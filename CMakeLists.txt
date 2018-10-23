cmake_minimum_required(VERSION 3.12)

# Set the default CMAKE_BUILD_TYPE to Release.
# This should be done before the project command since the latter can set
# CMAKE_BUILD_TYPE itself (it does so for nmake).
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
            "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel.")
endif ()

project(scratchbook)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(warnings)
include(downloadGtest)
include(downloadGSL)
include(downloadFmt)
include(downloadRapidJSON)

add_executable(scratchbook main.cpp art/Art.h art/Node4.h art/Node16.h art/Node48.h)
target_compile_options(scratchbook PRIVATE ${PEDANTIC_COMPILE_FLAGS})
target_compile_features(scratchbook PUBLIC cxx_std_17)
set_property(TARGET scratchbook PROPERTY CXX_EXTENSIONS OFF)
target_link_libraries(scratchbook GSL)
target_link_libraries(scratchbook gtest)
target_link_libraries(scratchbook fmt)
target_link_libraries(scratchbook RapidJSON)
