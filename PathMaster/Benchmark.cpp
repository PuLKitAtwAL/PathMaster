#include "Benchmark.h"

#include "BFS.h"
#include "Dijkstra.h"
#include "AStar.h"

#include <chrono>

BenchmarkResult Benchmark::runBFS(
    const Grid& grid)
{
    BFS bfs;

    auto start = std::chrono::high_resolution_clock::now();

    bool found = bfs.solve(grid);

    auto end = std::chrono::high_resolution_clock::now();

    double time =
        std::chrono::duration<double, std::milli>(
            end - start
        ).count();

    return {
        "BFS",
        found,
        static_cast<int>(
            bfs.getVisitedOrder().size()
        ),
        static_cast<int>(
            bfs.getPath().size()
        ),
        time
    };
}

BenchmarkResult Benchmark::runDijkstra(
    const Grid& grid)
{
    Dijkstra algorithm;

    auto start = std::chrono::high_resolution_clock::now();

    bool found = algorithm.solve(grid);

    auto end = std::chrono::high_resolution_clock::now();

    double time =
        std::chrono::duration<double, std::milli>(
            end - start
        ).count();

    return {
        "Dijkstra",
        found,
        static_cast<int>(
            algorithm.getVisitedOrder().size()
        ),
        static_cast<int>(
            algorithm.getPath().size()
        ),
        time
    };
}

BenchmarkResult Benchmark::runAStar(
    const Grid& grid)
{
    AStar algorithm;

    auto start = std::chrono::high_resolution_clock::now();

    bool found = algorithm.solve(grid);

    auto end = std::chrono::high_resolution_clock::now();

    double time =
        std::chrono::duration<double, std::milli>(
            end - start
        ).count();

    return {
        "A*",
        found,
        static_cast<int>(
            algorithm.getVisitedOrder().size()
        ),
        static_cast<int>(
            algorithm.getPath().size()
        ),
        time
    };
}