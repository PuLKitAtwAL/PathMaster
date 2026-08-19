# PathMaster

A C++ pathfinding visualizer built with SDL3 that lets you explore and compare **BFS, Dijkstra's Algorithm, and A\*** on randomly generated weighted mazes.

The project is designed to make pathfinding algorithms easier to understand by showing how they explore a grid and how the final path is selected.

## Features

* Visual grid-based maze
* Random maze generation with multiple possible routes
* Start and target nodes
* Four-direction movement
* Walls and weighted cells
* Weight `5` and `10` displayed directly on the grid
* BFS pathfinding
* Dijkstra's shortest-path algorithm
* A\* pathfinding with Manhattan-distance heuristic
* Animated algorithm exploration
* Adjustable animation speed
* Clickable algorithm controls
* Run-all comparison in the console
* Visited-node and final-path visualization
* Basic path/visited statistics
* Manual maze editing

## Algorithms

### Breadth-First Search (BFS)

BFS explores the grid level by level and is useful for finding a shortest path when every movement has the same cost.

**Time complexity:** `O(V + E)`

### Dijkstra's Algorithm

Dijkstra's algorithm considers the cost of reaching each cell, making it suitable for weighted grids.

In PathMaster:

* Normal cell = `1`
* Weighted cell = `5`
* Weighted cell = `10`
* Wall = blocked

**Time complexity:** typically `O((V + E) log V)` with the priority queue implementation used here.

### A\*

A\* combines the cost already travelled with a heuristic estimate of the remaining distance.

PathMaster uses the Manhattan-distance heuristic because movement is restricted to four directions.

**Evaluation:**

```text
f(n) = g(n) + h(n)
```

where:

* `g(n)` = cost from the start
* `h(n)` = estimated cost to the target

## Visualization

The grid uses a deliberately simple visual design:

|Element|Meaning|
|-|-|
|Green circle|Start|
|Red circle|Target|
|Dark cell|Wall|
|`5`|Movement cost 5|
|`10`|Movement cost 10|
|Blue|Visited/search area|
|Yellow|Final path|

Normal cells have a movement cost of `1`, so `1` is intentionally not displayed.

## Controls

### Application buttons

* **BFS** — animate Breadth-First Search
* **DIJKSTRA** — animate Dijkstra
* **A STAR** — animate A\*
* **RUN ALL** — compare the three algorithms in the console
* **NEW MAZE** — generate another random maze
* **CLEAR** — clear the current search/path visualization
* **RESET** — reset the grid
* **SLOWER / FASTER** — change animation speed

### Grid editing

* **Left click** — toggle a wall
* **Right click** — move the start node
* **Shift + Right click** — move the target node

### Keyboard shortcuts

```text
N = New Maze
R = Clear Search
C = Reset
```

## Tech Stack

* **C++**
* **SDL3**
* **Visual Studio**
* STL containers and algorithms
* Priority Queue
* Breadth-First Search
* Dijkstra's Algorithm
* A\* Search

## Project Structure

```text
PathMaster/
├── AStar.cpp
├── AStar.h
├── BFS.cpp
├── BFS.h
├── Dijkstra.cpp
├── Dijkstra.h
├── Grid.cpp
├── Grid.h
├── Benchmark.cpp
├── Benchmark.h
├── main.cpp
└── PathMaster.vcxproj
```

## How It Works

1. Generate a random maze.
2. Choose an algorithm.
3. The algorithm calculates its search order and final path.
4. PathMaster replays the search visually, one cell at a time.
5. The final path is highlighted after exploration.
6. Use **RUN ALL** to compare the algorithms.

## Building

### Requirements

* Windows
* Visual Studio 2022 or compatible Visual Studio setup
* SDL3 development libraries configured for the project

### Build

Open:

```text
PathMaster.slnx
```

in Visual Studio and use:

```text
Build → Rebuild Solution
```

Then run the application.

## Why This Project?

PathMaster was built as a practical way to study graph traversal and shortest-path algorithms rather than only implementing them as console programs.

It focuses on making the differences between BFS, Dijkstra, and A\* visible:

* How many cells each algorithm explores
* How weighted terrain affects path selection
* How the heuristic changes A\*'s search behaviour
* How different algorithms can reach the same target through different search strategies

## Current Status

The `animation` branch contains the current development version with the visual interface, weighted maze, and algorithm animation.

The project is actively being improved.

## Future Improvements

Possible future additions include:

* Side-by-side animated algorithm comparison
* More advanced maze-generation algorithms
* Better performance benchmarking
* Additional pathfinding algorithms
* Maze save/load
* Improved UI and visualization controls

## Author

**Pulkit Atwal**

GitHub: [@PuLKitAtwAL](https://github.com/PuLKitAtwAL)

\---

Built with C++ and SDL3.






## Screenshots



\### PathMaster UI



!\[PathMaster UI](assets/pathmaster-ui-clean.png)



\### A\* Visualization



!\[A\* Visualization](assets/pathmaster-ui-astar.png)



\### Algorithm Comparison



!\[Algorithm Comparison](assets/pathmaster-comparison.png)

