#pragma once

#include "Grid.h"
#include <string>

struct BenchmarkResult
{
    std::string algorithm;
    bool foundPath;
    int visitedNodes;
    int pathLength;
    double executionTimeMs;
};

class Benchmark
{
public:
    static BenchmarkResult runBFS(const Grid& grid);
    static BenchmarkResult runDijkstra(const Grid& grid);
    static BenchmarkResult runAStar(const Grid& grid);
};