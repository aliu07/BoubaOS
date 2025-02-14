cmake_minimum_required(VERSION 3.10)
project(BoubaOS C)

# Specify C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set build directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include/core
    ${PROJECT_SOURCE_DIR}/include/memory
    ${PROJECT_SOURCE_DIR}/include/scheduling
)

# Create executable
add_executable(mysh
    src/core/shell.c
    src/core/interpreter.c
    src/memory/shellmemory.c
    src/scheduling/pcb.c
    src/scheduling/ready_queue.c
    src/scheduling/scheduler.c
)
