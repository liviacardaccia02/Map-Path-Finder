cmake_minimum_required(VERSION 3.10)
project(MapPathFinder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add source files
set(SOURCES
    Graph.cpp
    GraphicGraph.cpp
    utils.cpp
    main.cpp
    algorithms.cpp
)

# Specify the Qt6 installation path
set(CMAKE_PREFIX_PATH "/usr/lib/qt6")

# Find Qt6 Widgets
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Create the executable
add_executable(graph_traversal ${SOURCES})

# Link Qt6::Widgets
target_link_libraries(graph_traversal PRIVATE Qt6::Widgets)

# Compilation flags
target_compile_options(graph_traversal PRIVATE -Wall -Wextra -pedantic)