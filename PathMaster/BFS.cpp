#include "BFS.h"

#include <queue>
#include <algorithm>

bool BFS::solve(const Grid& grid)
{
    visitedOrder.clear();
    path.clear();

    int rows = grid.getRows();
    int cols = grid.getCols();

    int startRow = grid.getStartRow();
    int startCol = grid.getStartCol();

    int targetRow = grid.getTargetRow();
    int targetCol = grid.getTargetCol();

    std::vector<std::vector<bool>> visited(
        rows,
        std::vector<bool>(cols, false)
    );

    std::vector<std::vector<std::pair<int, int>>> parent(
        rows,
        std::vector<std::pair<int, int>>(
            cols,
            { -1, -1 }
        )
    );

    std::queue<std::pair<int, int>> q;

    q.push({ startRow, startCol });
    visited[startRow][startCol] = true;

    int directions[4][2] =
    {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    while (!q.empty())
    {
        auto current = q.front();
        q.pop();

        int row = current.first;
        int col = current.second;

        visitedOrder.push_back({ row, col });

        if (row == targetRow && col == targetCol)
        {
            break;
        }

        for (auto& direction : directions)
        {
            int newRow = row + direction[0];
            int newCol = col + direction[1];

            if (!grid.isValidCell(newRow, newCol))
                continue;

            if (visited[newRow][newCol])
                continue;

            if (grid.isWall(newRow, newCol))
                continue;

            visited[newRow][newCol] = true;

            parent[newRow][newCol] =
            { row, col };

            q.push({ newRow, newCol });
        }
    }

    if (!visited[targetRow][targetCol])
    {
        return false;
    }

    int row = targetRow;
    int col = targetCol;

    while (!(row == startRow && col == startCol))
    {
        path.push_back({ row, col });

        auto parentCell = parent[row][col];

        row = parentCell.first;
        col = parentCell.second;
    }

    path.push_back({ startRow, startCol });

    std::reverse(path.begin(), path.end());

    return true;
}

const std::vector<std::pair<int, int>>&
BFS::getVisitedOrder() const
{
    return visitedOrder;
}

const std::vector<std::pair<int, int>>&
BFS::getPath() const
{
    return path;
}