#include "Grid.h"

#include <random>

Grid::Grid(int rows, int cols, int cellSize)
{
    this->rows = rows;
    this->cols = cols;
    this->cellSize = cellSize;

    cells.resize(rows);

    for (int r = 0; r < rows; r++)
    {
        cells[r].resize(cols);

        for (int c = 0; c < cols; c++)
        {
            cells[r][c] = {
                TerrainType::GRASS,
                CellState::NORMAL
            };
        }
    }

    startRow = 1;
    startCol = 1;

    targetRow = rows - 2;
    targetCol = cols - 2;

    cells[startRow][startCol].state = CellState::START;
    cells[targetRow][targetCol].state = CellState::TARGET;
}

bool Grid::isValidCell(int row, int col) const
{
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

bool Grid::isWall(int row, int col) const
{
    return !isValidCell(row, col) ||
           cells[row][col].terrain == TerrainType::WALL;
}

int Grid::getWeight(int row, int col) const
{
    if (!isValidCell(row, col))
        return 1000000;

    switch (cells[row][col].terrain)
    {
    case TerrainType::GRASS: return 1;
    case TerrainType::MUD:   return 5;
    case TerrainType::WATER: return 10;
    case TerrainType::WALL:  return 1000000;
    }

    return 1;
}

TerrainType Grid::getTerrain(int row, int col) const
{
    if (!isValidCell(row, col))
        return TerrainType::WALL;

    return cells[row][col].terrain;
}

void Grid::setTerrain(int row, int col, TerrainType terrain)
{
    if (!isValidCell(row, col))
        return;

    if ((row == startRow && col == startCol) ||
        (row == targetRow && col == targetCol))
        return;

    cells[row][col].terrain = terrain;
    cells[row][col].state = CellState::NORMAL;
}

void Grid::handleLeftClick(int row, int col)
{
    if (!isValidCell(row, col))
        return;

    if ((row == startRow && col == startCol) ||
        (row == targetRow && col == targetCol))
        return;

    if (cells[row][col].terrain == TerrainType::WALL)
        cells[row][col].terrain = TerrainType::GRASS;
    else
        cells[row][col].terrain = TerrainType::WALL;

    cells[row][col].state = CellState::NORMAL;
}

void Grid::handleRightClick(int row, int col, bool shiftPressed)
{
    if (!isValidCell(row, col))
        return;

    if (shiftPressed)
    {
        if (row == startRow && col == startCol)
            return;

        cells[targetRow][targetCol].state = CellState::NORMAL;
        cells[targetRow][targetCol].terrain = TerrainType::GRASS;

        targetRow = row;
        targetCol = col;

        cells[targetRow][targetCol].terrain = TerrainType::GRASS;
        cells[targetRow][targetCol].state = CellState::TARGET;
        return;
    }

    if (row == targetRow && col == targetCol)
        return;

    cells[startRow][startCol].state = CellState::NORMAL;
    cells[startRow][startCol].terrain = TerrainType::GRASS;

    startRow = row;
    startCol = col;

    cells[startRow][startCol].terrain = TerrainType::GRASS;
    cells[startRow][startCol].state = CellState::START;
}

void Grid::drawCircle(SDL_Renderer* renderer, float cx, float cy, float radius)
{
    int r = static_cast<int>(radius);

    for (int y = -r; y <= r; y++)
    {
        for (int x = -r; x <= r; x++)
        {
            if (x * x + y * y <= r * r)
            {
                SDL_FRect p{
                    cx + static_cast<float>(x),
                    cy + static_cast<float>(y),
                    1.0f,
                    1.0f
                };
                SDL_RenderFillRect(renderer, &p);
            }
        }
    }
}

void Grid::draw(SDL_Renderer* renderer)
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            SDL_FRect cell{
                static_cast<float>(c * cellSize),
                static_cast<float>(r * cellSize),
                static_cast<float>(cellSize),
                static_cast<float>(cellSize)
            };

            // Simple base
            SDL_SetRenderDrawColor(renderer, 52, 52, 58, 255);
            SDL_RenderFillRect(renderer, &cell);

            // Walls
            if (cells[r][c].terrain == TerrainType::WALL)
            {
                SDL_SetRenderDrawColor(renderer, 18, 18, 22, 255);
                SDL_RenderFillRect(renderer, &cell);
            }

            // Visited
            if (cells[r][c].state == CellState::VISITED)
            {
                SDL_SetRenderDrawColor(renderer, 75, 145, 210, 255);
                SDL_RenderFillRect(renderer, &cell);
            }

            // Final path
            if (cells[r][c].state == CellState::PATH)
            {
                SDL_SetRenderDrawColor(renderer, 235, 200, 50, 255);
                SDL_RenderFillRect(renderer, &cell);
            }

            // Grid
            SDL_SetRenderDrawColor(renderer, 100, 100, 105, 255);
            SDL_RenderRect(renderer, &cell);

            // Start
            if (cells[r][c].state == CellState::START)
            {
                SDL_SetRenderDrawColor(renderer, 40, 230, 100, 255);
                drawCircle(
                    renderer,
                    cell.x + cell.w / 2.0f,
                    cell.y + cell.h / 2.0f,
                    cellSize * 0.30f
                );
            }

            // Target
            if (cells[r][c].state == CellState::TARGET)
            {
                SDL_SetRenderDrawColor(renderer, 240, 60, 70, 255);
                drawCircle(
                    renderer,
                    cell.x + cell.w / 2.0f,
                    cell.y + cell.h / 2.0f,
                    cellSize * 0.30f
                );
            }

            // Only show 5 and 10. Normal weight 1 is intentionally blank.
            if (cells[r][c].terrain == TerrainType::MUD)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDebugText(
                    renderer,
                    cell.x + 16.0f,
                    cell.y + 14.0f,
                    "5"
                );
            }
            else if (cells[r][c].terrain == TerrainType::WATER)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDebugText(
                    renderer,
                    cell.x + 10.0f,
                    cell.y + 14.0f,
                    "10"
                );
            }
        }
    }
}

int Grid::getRows() const { return rows; }
int Grid::getCols() const { return cols; }
int Grid::getStartRow() const { return startRow; }
int Grid::getStartCol() const { return startCol; }
int Grid::getTargetRow() const { return targetRow; }
int Grid::getTargetCol() const { return targetCol; }

void Grid::clearSearch()
{
    for (auto& row : cells)
    {
        for (auto& cell : row)
        {
            if (cell.state == CellState::VISITED ||
                cell.state == CellState::PATH)
                cell.state = CellState::NORMAL;
        }
    }

    cells[startRow][startCol].state = CellState::START;
    cells[targetRow][targetCol].state = CellState::TARGET;
}

void Grid::markVisited(int row, int col)
{
    if (!isValidCell(row, col))
        return;

    if (cells[row][col].state == CellState::START ||
        cells[row][col].state == CellState::TARGET)
        return;

    cells[row][col].state = CellState::VISITED;
}

void Grid::markPath(int row, int col)
{
    if (!isValidCell(row, col))
        return;

    if (cells[row][col].state == CellState::START ||
        cells[row][col].state == CellState::TARGET)
        return;

    cells[row][col].state = CellState::PATH;
}

void Grid::clearAll()
{
    for (auto& row : cells)
    {
        for (auto& cell : row)
        {
            cell.terrain = TerrainType::GRASS;
            cell.state = CellState::NORMAL;
        }
    }

    startRow = 1;
    startCol = 1;
    targetRow = rows - 2;
    targetCol = cols - 2;

    cells[startRow][startCol].state = CellState::START;
    cells[targetRow][targetCol].state = CellState::TARGET;
}

void Grid::generateRandomMaze(unsigned int seed)
{
    std::mt19937 rng(seed);

    // Start with walls.
    for (auto& row : cells)
    {
        for (auto& cell : row)
        {
            cell.terrain = TerrainType::WALL;
            cell.state = CellState::NORMAL;
        }
    }

    startRow = 1;
    startCol = 1;
    targetRow = rows - 2;
    targetCol = cols - 2;

    // Main top-then-right route.
    int r = startRow;
    int c = startCol;

    cells[r][c].terrain = TerrainType::GRASS;

    while (c < targetCol)
    {
        ++c;
        cells[r][c].terrain = TerrainType::GRASS;
    }

    while (r < targetRow)
    {
        ++r;
        cells[r][c].terrain = TerrainType::GRASS;
    }

    // Second route: down, then right.
    r = startRow;
    c = startCol;

    while (r < targetRow)
    {
        ++r;
        cells[r][c].terrain = TerrainType::GRASS;
    }

    while (c < targetCol)
    {
        ++c;
        cells[r][c].terrain = TerrainType::GRASS;
    }

    // Third route: a middle detour.
    r = startRow;
    c = startCol;

    while (c < 4 && c < targetCol)
    {
        ++c;
        cells[r][c].terrain = TerrainType::GRASS;
    }

    while (r < targetRow)
    {
        ++r;
        cells[r][c].terrain = TerrainType::GRASS;
    }

    while (c < targetCol)
    {
        ++c;
        cells[r][c].terrain = TerrainType::GRASS;
    }

    // Add a few natural connections.
    std::uniform_int_distribution<int> rd(1, rows - 2);
    std::uniform_int_distribution<int> cd(1, cols - 2);

    for (int i = 0; i < 12; i++)
    {
        int rr = rd(rng);
        int cc = cd(rng);

        if (cells[rr][cc].terrain != TerrainType::WALL)
            continue;

        int neighbours = 0;

        if (rr > 0 && cells[rr - 1][cc].terrain != TerrainType::WALL) neighbours++;
        if (rr + 1 < rows && cells[rr + 1][cc].terrain != TerrainType::WALL) neighbours++;
        if (cc > 0 && cells[rr][cc - 1].terrain != TerrainType::WALL) neighbours++;
        if (cc + 1 < cols && cells[rr][cc + 1].terrain != TerrainType::WALL) neighbours++;

        if (neighbours >= 2)
            cells[rr][cc].terrain = TerrainType::GRASS;
    }

    // Add a few 5 and 10 cells only on open cells.
    std::uniform_int_distribution<int> chance(0, 99);
    int weighted = 0;

    for (int attempts = 0; attempts < 150 && weighted < 6; attempts++)
    {
        int rr = rd(rng);
        int cc = cd(rng);

        if (cells[rr][cc].terrain != TerrainType::GRASS)
            continue;

        if ((rr == startRow && cc == startCol) ||
            (rr == targetRow && cc == targetCol))
            continue;

        cells[rr][cc].terrain =
            (chance(rng) < 65)
            ? TerrainType::MUD
            : TerrainType::WATER;

        weighted++;
    }

    cells[startRow][startCol].terrain = TerrainType::GRASS;
    cells[startRow][startCol].state = CellState::START;

    cells[targetRow][targetCol].terrain = TerrainType::GRASS;
    cells[targetRow][targetCol].state = CellState::TARGET;
}

Cell Grid::getCell(int row, int col) const
{
    if (!isValidCell(row, col))
        return { TerrainType::WALL, CellState::NORMAL };

    return cells[row][col];
}
