cmake_minimum_required(VERSION 3.10)

project(YourProjectName)

set(CMAKE_C_STANDARD 11)

# Create the 'compiler' target
add_executable(compiler
    compiler/main.c
    compiler/helpers.c
    compiler/interpreter.c
    compiler/writer.c
)

# Set the output directory for the 'compiler' target
set_target_properties(compiler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Optional: Specify a separate directory for intermediate build files (e.g., object files)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add an install rule to copy the 'compiler' executable to the 'bin' directory
install(TARGETS compiler
    DESTINATION bin
)