include(warnings)

# add_executable with defaults for warnings and compile flags
function(add_executable_with_warnings name)
    # ARGN refers to additional arguments after 'name'
    add_executable(${name} ${ARGN})

    target_compile_options(${name} PRIVATE ${PEDANTIC_COMPILE_FLAGS})
    target_compile_features(${name} PUBLIC cxx_std_17)
    set_target_properties(${name} PROPERTIES CXX_EXTENSIONS OFF)
endfunction(add_executable_with_warnings)

# Aggressive warning settings can cause noisy warnings in dependency includes
# This function silences those warnings by rewriting the include directories to be considered SYSTEM
# see: https://stackoverflow.com/a/52136398
function(target_link_libraries_dont_warn target)
    set(libs ${ARGN})
    foreach(lib ${libs})
        get_target_property(lib_include_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES)
        target_include_directories(${target} SYSTEM PRIVATE ${lib_include_dirs})
        target_link_libraries(${target} ${lib})
    endforeach(lib)
endfunction(target_link_libraries_dont_warn)
