#include "Dijkstra.h"

#include <queue>
#include <vector>
#include <algorithm>
#include <limits>

struct DijkstraNode
{
    int distance;
    int row;
    int col;

    bool operator>(const DijkstraNode& other) const
    {
        return distance > other.distance;
    }
};

bool Dijkstra::solve(const Grid& grid)
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

    std::vector<std::vector<int>> dist(
        rows, std::vector<int>(cols, INF)
    );

    std::vector<std::vector<std::pair<int, int>>> parent(
        rows,
        std::vector<std::pair<int, int>>(cols, {-1, -1})
    );

    std::priority_queue<
        DijkstraNode,
        std::vector<DijkstraNode>,
        std::greater<DijkstraNode>
    > pq;

    dist[sr][sc] = 0;
    pq.push({0, sr, sc});

    const int dirs[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    while (!pq.empty())
    {
        DijkstraNode cur = pq.top();
        pq.pop();

        if (cur.distance != dist[cur.row][cur.col])
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

            int nd = cur.distance + grid.getWeight(nr, nc);

            if (nd < dist[nr][nc])
            {
                dist[nr][nc] = nd;
                parent[nr][nc] = {cur.row, cur.col};
                pq.push({nd, nr, nc});
            }
        }
    }

    if (dist[tr][tc] == INF)
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

const std::vector<std::pair<int, int>>& Dijkstra::getVisitedOrder() const
{
    return visitedOrder;
}

const std::vector<std::pair<int, int>>& Dijkstra::getPath() const
{
    return path;
}
