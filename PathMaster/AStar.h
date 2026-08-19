#pragma once

#include "Grid.h"

#include <vector>
#include <utility>

class AStar
{
private:
    std::vector<std::pair<int, int>>
        visitedOrder;

    std::vector<std::pair<int, int>>
        path;

public:
    bool solve(const Grid& grid);

    const std::vector<std::pair<int, int>>&
        getVisitedOrder() const;

    const std::vector<std::pair<int, int>>&
        getPath() const;
};