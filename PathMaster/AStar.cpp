#include "AStar.h"

#include <queue>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

struct AStarNode
{
    int f;
    int g;
    int row;
    int col;

    bool operator>(
        const AStarNode& other) const
    {
        return f > other.f;
    }
};

static int heuristic(
    int row,
    int col,
    int targetRow,
    int targetCol)
{
    return
        std::abs(row - targetRow) +
        std::abs(col - targetCol);
}

bool AStar::solve(
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
        gScore(
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
        AStarNode,
        std::vector<AStarNode>,
        std::greater<AStarNode>
    > pq;

    gScore[startRow][startCol] = 0;

    pq.push({
        heuristic(
            startRow,
            startCol,
            targetRow,
            targetCol
        ),
        0,
        startRow,
        startCol
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
        AStarNode current =
            pq.top();

        pq.pop();

        if (current.g !=
            gScore[current.row][current.col])
        {
            continue;
        }

        visitedOrder.push_back({
            current.row,
            current.col
            });

        if (current.row == targetRow &&
            current.col == targetCol)
        {
            break;
        }

        for (auto& direction :
            directions)
        {
            int newRow =
                current.row +
                direction[0];

            int newCol =
                current.col +
                direction[1];

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

            int newG =
                current.g +
                grid.getWeight(
                    newRow,
                    newCol
                );

            if (newG <
                gScore[newRow][newCol])
            {
                gScore[newRow][newCol] =
                    newG;

                parent[newRow][newCol] =
                {
                    current.row,
                    current.col
                };

                int h =
                    heuristic(
                        newRow,
                        newCol,
                        targetRow,
                        targetCol
                    );

                pq.push({
                    newG + h,
                    newG,
                    newRow,
                    newCol
                    });
            }
        }
    }

    if (gScore[targetRow][targetCol] ==
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
AStar::getVisitedOrder() const
{
    return visitedOrder;
}

const std::vector<std::pair<int, int>>&
AStar::getPath() const
{
    return path;
}