#include "Dijkstra.h"

#include <queue>
#include <vector>
#include <algorithm>
#include <limits>

using Node =
std::pair<int, std::pair<int, int>>;

bool Dijkstra::solve(
    const Grid& grid)
{
    visitedOrder.clear();
    path.clear();

    int rows = grid.getRows();
    int cols = grid.getCols();

    int startRow =
        grid.getStartRow();

    int startCol =
        grid.getStartCol();

    int targetRow =
        grid.getTargetRow();

    int targetCol =
        grid.getTargetCol();

    const int INF =
        std::numeric_limits<int>::max();

    std::vector<std::vector<int>>
        distance(
            rows,
            std::vector<int>(
                cols,
                INF
            )
        );

    std::vector<std::vector<std::pair<int, int>>>
        parent(
            rows,
            std::vector<std::pair<int, int>>(
                cols,
                { -1, -1 }
            )
        );

    std::priority_queue<
        Node,
        std::vector<Node>,
        std::greater<Node>
    > pq;

    distance[startRow][startCol] = 0;

    pq.push({
        0,
        {startRow, startCol}
        });

    int directions[4][2] =
    {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    while (!pq.empty())
    {
        auto current =
            pq.top();

        pq.pop();

        int currentDistance =
            current.first;

        int row =
            current.second.first;

        int col =
            current.second.second;

        if (currentDistance !=
            distance[row][col])
        {
            continue;
        }

        visitedOrder.push_back({
            row,
            col
            });

        if (row == targetRow &&
            col == targetCol)
        {
            break;
        }

        for (auto& direction :
            directions)
        {
            int newRow =
                row + direction[0];

            int newCol =
                col + direction[1];

            if (!grid.isValidCell(
                newRow,
                newCol))
            {
                continue;
            }

            if (grid.isWall(
                newRow,
                newCol))
            {
                continue;
            }

            int newDistance =
                currentDistance +
                grid.getWeight(
                    newRow,
                    newCol
                );

            if (newDistance <
                distance[newRow][newCol])
            {
                distance[newRow][newCol] =
                    newDistance;

                parent[newRow][newCol] =
                {
                    row,
                    col
                };

                pq.push({
                    newDistance,
                    {newRow, newCol}
                    });
            }
        }
    }

    if (distance[targetRow][targetCol] ==
        INF)
    {
        return false;
    }

    int row = targetRow;
    int col = targetCol;

    while (!(row == startRow &&
        col == startCol))
    {
        path.push_back({
            row,
            col
            });

        auto parentCell =
            parent[row][col];

        row = parentCell.first;
        col = parentCell.second;
    }

    path.push_back({
        startRow,
        startCol
        });

    std::reverse(
        path.begin(),
        path.end()
    );

    return true;
}

const std::vector<std::pair<int, int>>&
Dijkstra::getVisitedOrder() const
{
    return visitedOrder;
}

const std::vector<std::pair<int, int>>&
Dijkstra::getPath() const
{
    return path;
}