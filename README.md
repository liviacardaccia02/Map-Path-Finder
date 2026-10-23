# Map Path Finder

A C++ pathfinding application that models a road network as a directed, weighted graph and compares **Breadth-First Search (BFS)**, **Dijkstra's algorithm**, and **A***. An optional Qt 6 interface visualizes the graph and highlights the selected route.

> Academic project. The repository demonstrates graph modelling, shortest-path algorithms, file parsing, and a small graphical visualization. Review the limitations below before treating it as a production routing system.

## Features

- Directed graph represented with vertices and adjacency lists.
- BFS for finding a route with the fewest edges in an unweighted interpretation of the graph.
- Dijkstra for minimum-cost paths with non-negative edge weights.
- A* search guided by a local projected-distance heuristic.
- CSV-like graph file parser with line-numbered diagnostics for malformed record prefixes.
- Text output showing the path, cumulative path costs, number of visited vertices, and elapsed search time.
- Optional Qt 6 visualization of vertices, edges, and the selected path.

## Algorithms

| Algorithm | Objective | Important assumption |
|---|---|---|
| BFS | Route with the fewest edges | Does not minimize total edge weight when weights differ |
| Dijkstra | Minimum total edge weight | Edge weights must be finite and non-negative |
| A* | Minimum-cost path guided by a heuristic | Edge weights must be compatible with the projected-distance heuristic for the shortest-path guarantee |

The default edge weight is computed using the project's local map projection. If explicit weights are provided, ensure they use compatible distance units and are not smaller than the geometric lower bound needed by A*. Arbitrary custom weights can invalidate A*'s optimality guarantee.

## Requirements

- C++ compiler with C++20 support
- CMake 3.10 or newer
- Qt 6 Widgets development package

The current CMake target links Qt 6 even when running in text mode, so Qt 6 is required to build the current executable.

### Ubuntu / Debian

Install the build tools and Qt development package:

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake qt6-base-dev
```

## Build

From the repository root:

```bash
cmake -S . -B build
cmake --build build --parallel
```

This creates the `graph_traversal` executable in the `build/` directory.

## Run

The command-line interface requires a start vertex, an end vertex, an algorithm, and an input graph file.

### Text mode

```bash
./build/graph_traversal \
  --start 86771 \
  --end 110636 \
  --algorithm astar \
  --file graph_dc_area.2022-03-11.txt \
  --mode text
```

Supported algorithm values are `bfs`, `dijkstra`, and `astar`. The default mode is `text`, so `--mode text` can be omitted.

### Graphical mode

```bash
./build/graph_traversal \
  --start 86771 \
  --end 110636 \
  --algorithm astar \
  --file graph_dc_area.2022-03-11.txt \
  --mode graphic
```

Graphical mode requires a desktop environment with a working Qt platform plugin. It may not launch in a headless terminal or CI runner without a display server.

## Input file format

The parser reads one vertex or edge record per line. Lines beginning with `#` and empty lines are ignored. Define vertices before any edges that reference them.

### Vertex record

```text
V,<vertex_id>,<longitude>,<latitude>
```

### Edge record

```text
E,<source_vertex_id>,<destination_vertex_id>,<weight>
```

Edges are directed: `E,1,2,10` creates an edge from vertex 1 to vertex 2, not automatically from 2 to 1. If the edge weight is omitted or set to `0`, the program computes a projected Euclidean distance between the two vertices.

A small example:

```text
V,1,-73.9857,40.7484
V,2,-73.9851,40.7490
V,3,-73.9845,40.7495
E,1,2,
E,2,3,
E,1,3,
```

Save the records to `example.csv`, then run:

```bash
./build/graph_traversal --start 1 --end 3 --algorithm dijkstra --file example.csv
```

## Output

For a successful search, text mode reports:

- Number of visited vertices
- Number of vertices in the route
- Route vertex IDs and cumulative path cost
- Approximate elapsed search time

BFS selects by edge count, so the displayed cumulative weight is the cost of the chosen BFS route—not necessarily the minimum-weight route.

## Tests

The CLI regression suite checks sink destinations, BFS edge-count behavior versus weighted shortest paths, minimum-cost route selection by Dijkstra and A*, unreachable destinations, identical start/end vertices, and rejection of malformed records and invalid edge weights.

After building, run:

```bash
bash tests/smoke_test.sh ./build/graph_traversal
```

A GitHub Actions workflow configures and builds the project and runs the CLI regression suite on pushes and pull requests.

## Implementation notes and limitations

- This is an educational pathfinding project, not a production navigation system.
- BFS optimizes edge count rather than weighted distance.
- A*'s shortest-path guarantee depends on edge weights being compatible with its heuristic; arbitrary explicit weights may violate that assumption.
- The GUI is optional at runtime but Qt 6 is currently a build dependency.
- The CLI tests are focused regression checks, not exhaustive algorithm verification. They do not replace broader unit tests or property-based testing.
- The input parser expects a simple comma-separated format; it is not a general CSV parser with quoted fields.

## Project structure

- `main.cpp` — command-line arguments and application entry point
- `Graph.h / Graph.cpp` — graph data model, parsing, vertices, and adjacency lists
- `Vertex.h`, `Edge.h` — graph primitives
- `algorithms.h / algorithms.cpp` — BFS, Dijkstra, and A*
- `utils.h / utils.cpp` — coordinate projection and distance calculations
- `GraphicGraph.h / GraphicGraph.cpp` — Qt graph rendering
- `CMakeLists.txt` — build configuration
- `tests/` — small fixtures and CLI smoke tests

## Author

Livia Cardaccia
