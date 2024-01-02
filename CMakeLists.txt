cmake_minimum_required(VERSION 3.10)

project(SimonCompiler)

set(CMAKE_C_STANDARD 11)

# Add the executable
add_executable(compiler
    compiler/main.c
    compiler/helpers.c
    compiler/library_manager.c
    compiler/interpreter.c
    compiler/writer.c
)
set_target_properties(compiler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
install(TARGETS compiler
    DESTINATION bin
)

# Add the standard library
add_library(simon SHARED
    libraries/simon/simon.c
)
set_target_properties(simon PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/libraries
)
install(TARGETS simon
    LIBRARY DESTINATION bin/libraries
)