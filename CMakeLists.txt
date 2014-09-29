cmake_minimum_required(VERSION 3.0)
enable_testing()

# Sets CODE_SAMPLES and creates the samples target.
add_subdirectory(code)

# Creates the sampler executable.
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