



# Graph Project


**This project was carried out as part of a bachelor's degree in Computer and Software Engineering at Ariel University.** The goal is to implement core data structures and graph algorithms in C++, translating theoretical concepts into robust, tested code.

---

## Overview

The assignment focuses on an **undirected**, **weighted** graph and the following algorithms:

- **BFS** (Breadth-First Search)
- **DFS** (Depth-First Search)
- **Dijkstra** (Single-Source Shortest Paths)
- **Prim** (Minimum Spanning Tree)
- **Kruskal** (Minimum Spanning Tree)

Each algorithm produces a new `Graph` representing either a traversal tree or an MST/shortest-path tree.

---

## Project Structure

CPP_EX1_25/
├─ src/
│  ├─ Algorithms.cpp / Algorithms.hpp
│  ├─ doctest.h
|  ├─ EdgeMinHeap.hpp
│  ├─ Graph.cpp / graph.hpp
│  ├─ GraphTest.cpp
│  ├─ main.cpp
|  ├─ Makefile
|  ├─ MinHeap.hpp
│  ├─ QForAlg.cpp / QForAlg.hpp
├──└─ VertexMinHeap.hpp
├── README.md
└── submission.txt

---

## File Descriptions

### `Graph.hpp`

Defines the core graph data structures:

- **`Vertex`**: holds an integer ID and pointer to its outgoing edges.  
- **`Edge`**: stores weight, pointers to its two endpoint `Vertex` objects, and the next edge in the adjacency list.  
- **`Graph`**: encapsulates a fixed-size array of `Vertex*`, with methods to add/remove edges and to query adjacency.

### `Algorithms.hpp`

Declares the static `Algorithms` class with methods:

```cpp
static Graph* bfs(const Graph& g, int source);
static Graph* dfs(const Graph& g, int source);
static Graph* dijkstra(const Graph& g, int source);
static Graph* prim(const Graph& g);
static Graph* kruskal(const Graph& g);


```

---

## File Descriptions

Below is a concise overview of each key file in the project:

- src\Graph.hpp
  Defines the core graph structures: `Vertex`, `Edge`, and the `Graph` class, including constructors, destructors, and adjacency-list management.

- src\Algorithms.hpp
  Declares the static `Algorithms` class with methods:

  ```cpp
  static Graph* bfs(const Graph& g, int source);
  static Graph* dfs(const Graph& g, int source);
  static Graph* dijkstra(const Graph& g, int source);
  static Graph* prim(const Graph& g);
  static Graph* kruskal(const Graph& g);

  template<typename T, typename Compare>
  class MinHeap {
  // insert, extractMin, peek, isEmpty, isFull, decreaseKey (specialized via Compare)...
  };

  ```

- `` src\QForAlg.hpp
  Implements a simple FIFO queue with fixed capacity; 

- `` src\HeapMin.hpp
  Implements A generic, header-only template:

  ```cpp
  template<typename T, typename Compare>
  class MinHeap {
  // insert, extractMin, peek, isEmpty, isFull, decreaseKey (specialized via Compare)...
  };

  ```

- ``src\Graph.cpp
  Implements all `Graph`, `Vertex`, and `Edge` member functions and adjacency-list operations.

- ``src/Algorithms.cpp
  Provides full implementations of all five algorithms:

  - **BFS/DFS**: Builds a traversal tree with only tree edges.
  - **Dijkstra**: Maintains a distance array and uses `VertexMinHeap`.
  - **Prim**: Grows an MST via `VertexMinHeap`.
  - **Kruskal**: Sorts all edges and applies Union–Find to build an MST via `EdgeMinHeap`.

- `` src\main.cpp
  Demo harness: reads graph input, runs algorithms, and prints both textual and simple visual results.

- `` src\QForAlg.cpp
  Implements the simple functuons for a queue with fixed capacity; 


- src/VertexMinHeap.cpp & src/EdgeMinHeap.cpp
Type aliases over MinHeap:
   VertexMinHeap: keyed on Vertex::distance, used by Dijkstra and Prim.
   EdgeMinHeap: keyed on HeapEdge::weight, used by Kruskal.
(No separate .cpp—all inline in headers.)



- src\GraphTest.cpp
  Validates constructors, basic properties, and error handling.
  Covers all five algorithms, including edge cases.
  Ensures 0 memory leaks under Valgrind.
  Single-header testing framework (with DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN).

- src\doctest.h
  Single-header testing framework (with DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN).

---

## Build and Run

**Prerequisites:**

- **Compiler:** `g++` (C++17 or newer)
- **Build tool:** `make`

**Typical Workflow:**

```bash
$ make Main      # Compile and execute the demo (./Main)
$ make test      # Build and run all unit tests
$ make valgrind  # Run tests under Valgrind (zero leaks expected)
$ make clean     # Clean up build artifacts
```

---

## Testing & Memory Management

- **Unit Tests:** Cover all algorithms and data structures; located under `GraphTest.cpp`.
- **Valgrind:** Confirms 0 memory leaks across all code paths, demonstrating proper allocation and deallocation.

---


