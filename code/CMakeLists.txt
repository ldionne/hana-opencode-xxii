# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.

#=============================================================================
# Setup compiler flags for code samples
#=============================================================================
include(CheckCXXCompilerFlag)
macro(append_flag lst testname flag)
    check_cxx_compiler_flag(${flag} ${testname})
    if (${testname})
        list(APPEND ${lst} ${flag})
    endif()
endmacro()

append_flag(CXX_FLAGS HAS_W_FLAG                         -W)
append_flag(CXX_FLAGS HAS_WALL_FLAG                      -Wall)
append_flag(CXX_FLAGS HAS_WEXTRA_FLAG                    -Wextra)
append_flag(CXX_FLAGS HAS_WNO_LONG_LONG_FLAG             -Wno-long-long)
append_flag(CXX_FLAGS HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG -Wno-unused-local-typedefs)
append_flag(CXX_FLAGS HAS_WNO_UNUSED_PARAMETER_FLAG      -Wno-unused-parameter)
append_flag(CXX_FLAGS HAS_WWRITE_STRINGS_FLAG            -Wwrite-strings)
append_flag(CXX_FLAGS HAS_PEDANTIC_FLAG                  -pedantic)
check_cxx_compiler_flag(-std=c++1y HAS_STDCXX1Y_FLAG)

add_definitions(${CXX_FLAGS})
include_directories(ext/hana/include)


#=============================================================================
# Setup required packages
#=============================================================================
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

#=============================================================================
# Setup targets
#=============================================================================
add_custom_target(samples)

# CODE_SAMPLES is set in the parent directory
foreach(sample IN LISTS CODE_SAMPLES)
    file(RELATIVE_PATH sample_target ${CMAKE_CURRENT_SOURCE_DIR} ${sample})
    string(REPLACE ".cpp" "" sample_target "${sample_target}")
    add_executable(${sample_target} "${sample}")
    add_test(${sample_target} ${sample_target})
    add_dependencies(samples ${sample_target})

    if(NOT ${sample_target} MATCHES "_03" AND HAS_STDCXX1Y_FLAG)
        set_property(TARGET ${sample_target} APPEND
            PROPERTY COMPILE_OPTIONS "-std=c++1y")
    endif()
endforeach()