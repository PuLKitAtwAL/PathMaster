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

    bool operator>(const AStarNode& other) const
    {
        return f > other.f;
    }
};

static int heuristic(int r, int c, int tr, int tc)
{
    return std::abs(r - tr) + std::abs(c - tc);
}

bool AStar::solve(const Grid& grid)
{
    visitedOrder.clear();
    path.clear();

    const int rows = grid.getRows();
    const int cols = grid.getCols();

    const int sr = grid.getStartRow();
    const int sc = grid.getStartCol();
    const int tr = grid.getTargetRow();
    const int tc = grid.getTargetCol();

    const int INF = std::numeric_limits<int>::max();

    std::vector<std::vector<int>> gScore(
        rows, std::vector<int>(cols, INF)
    );

    std::vector<std::vector<std::pair<int, int>>> parent(
        rows,
        std::vector<std::pair<int, int>>(cols, {-1, -1})
    );

    std::priority_queue<
        AStarNode,
        std::vector<AStarNode>,
        std::greater<AStarNode>
    > pq;

    gScore[sr][sc] = 0;

    pq.push({
        heuristic(sr, sc, tr, tc),
        0,
        sr,
        sc
    });

    const int dirs[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    while (!pq.empty())
    {
        AStarNode cur = pq.top();
        pq.pop();

        if (cur.g != gScore[cur.row][cur.col])
            continue;

        visitedOrder.push_back({cur.row, cur.col});

        if (cur.row == tr && cur.col == tc)
            break;

        for (const auto& d : dirs)
        {
            int nr = cur.row + d[0];
            int nc = cur.col + d[1];

            if (!grid.isValidCell(nr, nc) || grid.isWall(nr, nc))
                continue;

            int newG = cur.g + grid.getWeight(nr, nc);

            if (newG < gScore[nr][nc])
            {
                gScore[nr][nc] = newG;
                parent[nr][nc] = {cur.row, cur.col};

                int h = heuristic(nr, nc, tr, tc);

                pq.push({
                    newG + h,
                    newG,
                    nr,
                    nc
                });
            }
        }
    }

    if (gScore[tr][tc] == INF)
        return false;

    int r = tr;
    int c = tc;

    while (!(r == sr && c == sc))
    {
        path.push_back({r, c});

        auto p = parent[r][c];
        if (p.first == -1)
            return false;

        r = p.first;
        c = p.second;
    }

    path.push_back({sr, sc});
    std::reverse(path.begin(), path.end());

    return true;
}

const std::vector<std::pair<int, int>>& AStar::getVisitedOrder() const
{
    return visitedOrder;
}

const std::vector<std::pair<int, int>>& AStar::getPath() const
{
    return path;
}
