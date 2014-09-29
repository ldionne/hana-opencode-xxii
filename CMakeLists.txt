# Adapted from
# http://github.com/tzlaine/type_erasure
#
# All credits to Zach Laine.

cmake_minimum_required(VERSION 3.0)
enable_testing()

file(GLOB CODE_SAMPLES code/*.cpp)
add_subdirectory(code)
add_subdirectory(sampler)

add_custom_command(
    OUTPUT
        index.html
    COMMAND
        sampler
        ${CMAKE_CURRENT_SOURCE_DIR}/index.in.html
        ${CMAKE_CURRENT_SOURCE_DIR}/index.html
        ${CODE_SAMPLES}
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/index.in.html
        samples
        sampler
    COMMENT
        "Generating index.html from index.in.html and source code samples."
)

add_custom_target(index DEPENDS index.html)