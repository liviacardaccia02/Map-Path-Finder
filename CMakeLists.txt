cmake_minimum_required(VERSION 3.10)
project(MapPathFinder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add source files
set(SOURCES
    Graph.cpp
    utils.cpp
    main.cpp
    algorithms.cpp
)

# Create the executable
add_executable(graph_traversal ${SOURCES})

# Compilation flags
target_compile_options(graph_traversal PRIVATE -Wall -Wextra -pedantic)