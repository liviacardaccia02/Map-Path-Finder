# 🗺️ Map Path Finder

## 📖 Introduction
This project implements **graph search algorithms** to find paths on a map, focusing on **shortest path algorithms** commonly used in navigation systems (e.g., car GPS, smartphone apps, Google Maps).

It is structured around core classes for graph representation and pathfinding, including:
- **Breadth-First Search (BFS)**
- **Dijkstra**
- **A\*** algorithms

The project abstracts a **road map** as a **directed graph**, where:
- **Vertices** represent road intersections.
- **Edges** represent road segments with weights for distances.

The **non-graphical part** covers graph creation, file reading, and algorithm implementation.  
The **optional graphical part** adds visualization using **Qt**.

> 🧠 *This is ROBO4 C++ Project #1, developed by Livia Cardaccia.*

---

## ⚙️ Main Features

### 🧩 Graph Representation
- **Edge class** — Encapsulates connections between vertices, storing start and end IDs along with a weight value for distance.  
- **Vertex class** — Represents individual nodes, holding an ID and geographic coordinates (longitude and latitude).  
- **Graph class** — Central structure managing collections of vertices and edges using `unordered_map` for efficient access and adjacency lists for neighbor retrieval.  

---

### 🎨 Graphical Visualization
- **GraphicGraph class** — Inherits from `Graph`, adding a `QGraphicsScene` pointer for rendering.  
  Overrides methods like `addVertex()` and `addEdge()` to create visual elements (ellipses for vertices and lines for edges) while preserving base logic.

---

### 🛠️ Utility Functions
- **utils namespace** — Provides coordinate projections and distance calculations.

---

### 🔍 Pathfinding Algorithms
- **algorithms namespace** — Implements BFS, Dijkstra, and A* for modular separation of concerns.  

**BFS:**  
Explores the graph level by level using a queue, suitable for unweighted graphs (or weighted with cumulative distances).  
Tracks visited vertices, parents for path reconstruction, and distances.  

**Dijkstra & A\*:**  
Handle weighted graphs to compute the shortest paths efficiently.

---

### 📂 File Reading
Supports reading graph map files in a **comma-separated value (CSV)** format:  
- Lines starting with **V** define vertices → ID, longitude, latitude.  
- Lines starting with **E** define edges → source ID, destination ID, length.
- An example of a graph map file is included in the repo (`graph_dc_area.2022-03-11.txt`).

---

### 📈 Results and Traces
Outputs include:
- Total visited vertices  
- Path vertices (with IDs)  
- Cumulative distances

---

## 🧮 Compilation

This project is written in **C++** and uses **Qt** for the optional graphical visualization (recommended Qt 5 or later).

### 🔹 Using CMake
    mkdir build
    cd build
    cmake ..
    make

### 🔹 Manual compilation with g++ (non-Qt version)
    g++ -std=c++11 -o graph_traversal main.cpp [other source files] \
        -I/usr/include/qt5 -lQt5Core -lQt5Gui -lQt5Widgets

> ⚠️ Adjust include paths and library flags based on your Qt installation.

**Note:**  
The project separates logic into namespaces (`utils`, `algorithms`), so remember to include all relevant source files.  
If you use a custom build system (e.g., Makefile), adjust accordingly.

---

## 🚀 Running the Project

The main executable is **graph_traversal**.  
Run it from the terminal with flags for:
- start vertex
- end vertex
- algorithm
- graph file

The graph file (e.g., `graph_dc_area.2022-03-11.txt`) must follow the CSV format described above.

---

### 🧭 Example Commands

#### 🔹 BFS
    ./graph_traversal --start 86771 --end 110636 --algorithm bfs --file graph_dc_area.2022-03-11.txt

Output includes total visited vertices, path vertices, and cumulative length.  
> Example: 28 vertices on the path (BFS); total length may vary slightly.

---

#### 🔹 Dijkstra
    ./graph_traversal --start 86771 --end 110636 --algorithm dijkstra --file graph_dc_area.2022-03-11.txt
> Example: 40 vertices on the path, total length ≈ 1940.29, 1785 vertices visted.

---

#### 🔹 A*
    ./graph_traversal --start 86771 --end 110636 --algorithm astar --file graph_dc_area.2022-03-11.txt
> Example: 40 vertices on the path, total length ≈ 1940.29, 369 vertices visted.

---

### 🎨 Optional Graphical Mode
If you compiled the **Qt version**, you can run the graphical executable to visualize:
- **Vertices** → drawn as ellipses  
- **Edges** → drawn as lines  

The shortest path will be rendered over the map interactively.

---

## 🧾 License
This project is distributed for educational purposes as part of the **ROBO4 C++ coursework**.

---

## 👩‍💻 Author
**Livia Cardaccia**  
C++ Developer & Robotics Engineering Student
```
