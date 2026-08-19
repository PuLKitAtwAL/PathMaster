#include "BFS.h"

#include <queue>
#include <vector>
#include <algorithm>

bool BFS::solve(const Grid& grid)
{
    visitedOrder.clear();
    path.clear();

    int rows = grid.getRows();
    int cols = grid.getCols();

    int sr = grid.getStartRow();
    int sc = grid.getStartCol();
    int tr = grid.getTargetRow();
    int tc = grid.getTargetCol();

    std::queue<std::pair<int, int>> q;

    std::vector<std::vector<bool>> visited(
        rows, std::vector<bool>(cols, false)
    );

    std::vector<std::vector<std::pair<int, int>>> parent(
        rows,
        std::vector<std::pair<int, int>>(cols, {-1, -1})
    );

    q.push({sr, sc});
    visited[sr][sc] = true;

    const int dirs[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    while (!q.empty())
    {
        auto [r, c] = q.front();
        q.pop();

        visitedOrder.push_back({r, c});

        if (r == tr && c == tc)
            break;

        for (const auto& d : dirs)
        {
            int nr = r + d[0];
            int nc = c + d[1];

            if (!grid.isValidCell(nr, nc) ||
                grid.isWall(nr, nc) ||
                visited[nr][nc])
            {
                continue;
            }

            visited[nr][nc] = true;
            parent[nr][nc] = {r, c};
            q.push({nr, nc});
        }
    }

    if (!visited[tr][tc])
        return false;

    int r = tr;
    int c = tc;

    while (!(r == sr && c == sc))
    {
        path.push_back({r, c});

        auto p = parent[r][c];
        r = p.first;
        c = p.second;
    }

    path.push_back({sr, sc});
    std::reverse(path.begin(), path.end());

    return true;
}

const std::vector<std::pair<int, int>>& BFS::getVisitedOrder() const
{
    return visitedOrder;
}

const std::vector<std::pair<int, int>>& BFS::getPath() const
{
    return path;
}
