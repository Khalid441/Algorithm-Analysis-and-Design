# Dynamic All-Pairs Shortest Path (APSP)

A C++ implementation of a **Dynamic All-Pairs Shortest Path** algorithm for a non-negative weighted directed graph.

The program maintains the shortest-distance matrix while the graph changes dynamically, without recalculating APSP from scratch after every update.

## Features

The program supports:

* Add a new edge
* Delete an existing edge
* Increase an edge weight
* Decrease an edge weight
* Query the shortest distance between two vertices
* Display the current graph
* Display the complete APSP distance matrix

## Approach

The initial APSP matrix is calculated using **Floyd-Warshall**.

After initialization, dynamic updates are handled as follows:

### 1. Edge Insertion

For a new edge:

```text
u → v with weight w
```

a possible new shortest path is:

```text
i → u → v → j
```

So the distance is updated using:

```text
D[i][j] = min(D[i][j],
              D[i][u] + w + D[v][j])
```

**Time Complexity:**

```text
O(V²)
```

### 2. Weight Decrease

When an edge weight decreases, the same relaxation is performed.

**Time Complexity:**

```text
O(V²)
```

This is faster than recalculating APSP from scratch.

### 3. Edge Deletion

When an edge is deleted, some existing shortest paths may become invalid.

The program first identifies potentially affected source vertices and then runs Dijkstra only from those sources.

If `A` source vertices are affected:

```text
O(V + A × E log V)
```

### 4. Weight Increase

Weight increase is handled similarly to edge deletion because previously shortest paths may no longer be optimal.

```text
O(V + A × E log V)
```

where `A` is the number of affected sources.

## Complexity Summary

| Operation       | Time Complexity    |
| --------------- | ------------------ |
| Initial APSP    | `O(V³)`            |
| Add Edge        | **`O(V²)`**        |
| Decrease Weight | **`O(V²)`**        |
| Delete Edge     | `O(V + A E log V)` |
| Increase Weight | `O(V + A E log V)` |
| Distance Query  | `O(1)`             |

For comparison, recomputing APSP using Johnson's algorithm after every update costs approximately:

```text
O(VE + V² log V)
```

Therefore, edge insertion and weight decrease can be handled more efficiently using the dynamic approach.

## Requirements

* C++17 or later
* Any standard C++ compiler

Compile using:

```bash
g++ -std=c++17 dynamic_apsp.cpp -o dynamic_apsp
```

Run:

```bash
./dynamic_apsp
```

On Windows:

```bash
dynamic_apsp.exe
```

## Input Format

First enter:

```text
Number of vertices
Number of edges
```

Then enter each edge as:

```text
u v weight
```

Example:

```text
4
5
0 1 5
1 2 3
0 2 12
2 3 2
0 3 20
```

The graph is **directed**, so:

```text
0 1 5
```

means:

```text
0 → 1 = 5
```

## Dynamic Operations

After the initial graph is loaded, the program provides a menu:

```text
1. Add Edge
2. Delete Edge
3. Increase Edge Weight
4. Decrease Edge Weight
5. Print Graph
6. Print APSP Matrix
7. Query Distance
8. Exit
```

### Example

To add:

```text
0 → 3 = 4
```

select:

```text
1
```

and enter:

```text
0 3 4
```

The distance matrix is updated dynamically.

## Algorithm Structure

```text
Initial Graph
     │
     ▼
Floyd-Warshall
     │
     ▼
Distance Matrix D
     │
     ├── Add Edge ───────────► O(V²) Update
     │
     ├── Decrease Weight ────► O(V²) Update
     │
     ├── Delete Edge ────────► Affected Sources
     │                              │
     │                              ▼
     │                         Dijkstra
     │
     └── Increase Weight ────► Affected Sources
                                    │
                                    ▼
                               Dijkstra
```

## Key Idea

The main idea of this project is:

> **Do not calculate APSP again if the existing shortest-path information can be reused.**

For insertion and weight decrease, only paths that can use the changed edge need to be checked. For deletion and weight increase, only potentially affected sources are recalculated.

## Limitations

* The graph must have **non-negative edge weights**.
* Deletion and weight increase can become expensive if many sources are affected.
* The initial APSP calculation requires `O(V³)` time.

## Project Structure

```text
Dynamic-APSP/
│
├── dynamic_apsp.cpp
└── README.md
```

## Author

**Khalid Bin Atik**

**Department of Computer Science and Engineering**
**Khulna University of Engineering & Technology (KUET)**

## License

This project is created for **academic and educational purposes**.
