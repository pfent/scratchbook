configure_and_build(googletest)

# Prevent overriding the parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/googletest
        ${CMAKE_BINARY_DIR}/googletest
        EXCLUDE_FROM_ALL)