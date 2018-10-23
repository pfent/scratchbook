include(warnings)

# add_executable with defaults for warnings and compile flags
function(add_executable_with_warnings name)
    # ARGN refers to additional arguments after 'name'
    add_executable(${name} ${ARGN})

    target_compile_options(${name} PRIVATE ${PEDANTIC_COMPILE_FLAGS})
    target_compile_features(${name} PUBLIC cxx_std_17)
    set_target_properties(${name} PROPERTIES CXX_EXTENSIONS OFF)
endfunction()
