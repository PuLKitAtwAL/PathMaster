#include "Grid.h"

#include <random>
#include <algorithm>

// ============================================================
// CONSTRUCTOR
// ============================================================

Grid::Grid(int rows, int cols, int cellSize)
{
    this->rows = rows;
    this->cols = cols;
    this->cellSize = cellSize;

    cells.resize(rows);

    for (int row = 0; row < rows; row++)
    {
        cells[row].resize(cols);

        for (int col = 0; col < cols; col++)
        {
            cells[row][col].terrain = TerrainType::GRASS;
            cells[row][col].state = CellState::NORMAL;
        }
    }

    startRow = 1;
    startCol = 1;

    targetRow = rows - 2;
    targetCol = cols - 2;

    cells[startRow][startCol].state = CellState::START;
    cells[targetRow][targetCol].state = CellState::TARGET;
}

// ============================================================
// VALIDATION
// ============================================================

bool Grid::isValidCell(int row, int col) const
{
    return row >= 0 &&
        row < rows &&
        col >= 0 &&
        col < cols;
}

bool Grid::isWall(int row, int col) const
{
    if (!isValidCell(row, col))
        return true;

    return cells[row][col].terrain == TerrainType::WALL;
}

// ============================================================
// WEIGHTS
// ============================================================

int Grid::getWeight(int row, int col) const
{
    if (!isValidCell(row, col))
        return 1000000;

    switch (cells[row][col].terrain)
    {
    case TerrainType::GRASS:
        return 1;

    case TerrainType::MUD:
        return 5;

    case TerrainType::WATER:
        return 10;

    case TerrainType::WALL:
        return 1000000;
    }

    return 1;
}

TerrainType Grid::getTerrain(int row, int col) const
{
    if (!isValidCell(row, col))
        return TerrainType::WALL;

    return cells[row][col].terrain;
}

void Grid::setTerrain(
    int row,
    int col,
    TerrainType terrain)
{
    if (!isValidCell(row, col))
        return;

    if ((row == startRow && col == startCol) ||
        (row == targetRow && col == targetCol))
    {
        return;
    }

    cells[row][col].terrain = terrain;
    cells[row][col].state = CellState::NORMAL;
}

// ============================================================
// MOUSE
// ============================================================

void Grid::handleLeftClick(int row, int col)
{
    if (!isValidCell(row, col))
        return;

    if ((row == startRow && col == startCol) ||
        (row == targetRow && col == targetCol))
    {
        return;
    }

    if (cells[row][col].terrain == TerrainType::WALL)
    {
        cells[row][col].terrain = TerrainType::GRASS;
    }
    else
    {
        cells[row][col].terrain = TerrainType::WALL;
    }

    cells[row][col].state = CellState::NORMAL;
}

void Grid::handleRightClick(
    int row,
    int col,
    bool shiftPressed)
{
    if (!isValidCell(row, col))
        return;

    // SHIFT + RIGHT CLICK = TARGET
    if (shiftPressed)
    {
        if (row == startRow && col == startCol)
            return;

        cells[targetRow][targetCol].state =
            CellState::NORMAL;

        cells[targetRow][targetCol].terrain =
            TerrainType::GRASS;

        targetRow = row;
        targetCol = col;

        cells[targetRow][targetCol].terrain =
            TerrainType::GRASS;

        cells[targetRow][targetCol].state =
            CellState::TARGET;

        return;
    }

    // RIGHT CLICK = START
    if (row == targetRow && col == targetCol)
        return;

    cells[startRow][startCol].state =
        CellState::NORMAL;

    cells[startRow][startCol].terrain =
        TerrainType::GRASS;

    startRow = row;
    startCol = col;

    cells[startRow][startCol].terrain =
        TerrainType::GRASS;

    cells[startRow][startCol].state =
        CellState::START;
}

// ============================================================
// DRAW CIRCLE
// ============================================================

void Grid::drawCircle(
    SDL_Renderer* renderer,
    float centerX,
    float centerY,
    float radius)
{
    int r = static_cast<int>(radius);

    for (int y = -r; y <= r; y++)
    {
        for (int x = -r; x <= r; x++)
        {
            if (x * x + y * y <= r * r)
            {
                SDL_FRect pixel;

                pixel.x =
                    centerX + static_cast<float>(x);

                pixel.y =
                    centerY + static_cast<float>(y);

                pixel.w = 1.0f;
                pixel.h = 1.0f;

                SDL_RenderFillRect(
                    renderer,
                    &pixel
                );
            }
        }
    }
}

// ============================================================
// DRAW GRID
// ============================================================

void Grid::draw(SDL_Renderer* renderer)
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            SDL_FRect cell;

            cell.x =
                static_cast<float>(
                    col * cellSize
                    );

            cell.y =
                static_cast<float>(
                    row * cellSize
                    );

            cell.w =
                static_cast<float>(cellSize);

            cell.h =
                static_cast<float>(cellSize);

            // ------------------------------------------------
            // NORMAL OPEN CELL
            // ------------------------------------------------

            SDL_SetRenderDrawColor(
                renderer,
                55,
                55,
                60,
                255
            );

            SDL_RenderFillRect(
                renderer,
                &cell
            );

            // ------------------------------------------------
            // WALL
            // ------------------------------------------------

            if (cells[row][col].terrain ==
                TerrainType::WALL)
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    18,
                    18,
                    22,
                    255
                );

                SDL_RenderFillRect(
                    renderer,
                    &cell
                );
            }

            // ------------------------------------------------
            // VISITED
            // ------------------------------------------------

            if (cells[row][col].state ==
                CellState::VISITED)
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    75,
                    145,
                    210,
                    255
                );

                SDL_RenderFillRect(
                    renderer,
                    &cell
                );
            }

            // ------------------------------------------------
            // PATH
            // ------------------------------------------------

            if (cells[row][col].state ==
                CellState::PATH)
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    235,
                    200,
                    50,
                    255
                );

                SDL_RenderFillRect(
                    renderer,
                    &cell
                );
            }

            // ------------------------------------------------
            // GRID BORDER
            // ------------------------------------------------

            SDL_SetRenderDrawColor(
                renderer,
                95,
                95,
                100,
                255
            );

            SDL_RenderRect(
                renderer,
                &cell
            );

            // ------------------------------------------------
            // START
            // ------------------------------------------------

            if (cells[row][col].state ==
                CellState::START)
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    40,
                    230,
                    100,
                    255
                );

                drawCircle(
                    renderer,
                    cell.x + cell.w / 2.0f,
                    cell.y + cell.h / 2.0f,
                    cellSize * 0.30f
                );
            }

            // ------------------------------------------------
            // TARGET
            // ------------------------------------------------

            if (cells[row][col].state ==
                CellState::TARGET)
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    240,
                    60,
                    70,
                    255
                );

                drawCircle(
                    renderer,
                    cell.x + cell.w / 2.0f,
                    cell.y + cell.h / 2.0f,
                    cellSize * 0.30f
                );
            }

            // ------------------------------------------------
            // WEIGHT 5
            // ------------------------------------------------

            if (cells[row][col].terrain ==
                TerrainType::MUD)
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    255,
                    255,
                    255,
                    255
                );

                SDL_RenderDebugText(
                    renderer,
                    cell.x + 16.0f,
                    cell.y + 14.0f,
                    "5"
                );
            }

            // ------------------------------------------------
            // WEIGHT 10
            // ------------------------------------------------

            if (cells[row][col].terrain ==
                TerrainType::WATER)
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    255,
                    255,
                    255,
                    255
                );

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

// ============================================================
// GETTERS
// ============================================================

int Grid::getRows() const
{
    return rows;
}

int Grid::getCols() const
{
    return cols;
}

int Grid::getStartRow() const
{
    return startRow;
}

int Grid::getStartCol() const
{
    return startCol;
}

int Grid::getTargetRow() const
{
    return targetRow;
}

int Grid::getTargetCol() const
{
    return targetCol;
}

// ============================================================
// CLEAR SEARCH
// ============================================================

void Grid::clearSearch()
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (cells[row][col].state ==
                CellState::VISITED ||
                cells[row][col].state ==
                CellState::PATH)
            {
                cells[row][col].state =
                    CellState::NORMAL;
            }
        }
    }

    cells[startRow][startCol].state =
        CellState::START;

    cells[targetRow][targetCol].state =
        CellState::TARGET;
}

// ============================================================
// MARK VISITED
// ============================================================

void Grid::markVisited(
    int row,
    int col)
{
    if (!isValidCell(row, col))
        return;

    if (cells[row][col].state ==
        CellState::START ||
        cells[row][col].state ==
        CellState::TARGET)
    {
        return;
    }

    cells[row][col].state =
        CellState::VISITED;
}

// ============================================================
// MARK PATH
// ============================================================

void Grid::markPath(
    int row,
    int col)
{
    if (!isValidCell(row, col))
        return;

    if (cells[row][col].state ==
        CellState::START ||
        cells[row][col].state ==
        CellState::TARGET)
    {
        return;
    }

    cells[row][col].state =
        CellState::PATH;
}

// ============================================================
// CLEAR EVERYTHING
// ============================================================

void Grid::clearAll()
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            cells[row][col].terrain =
                TerrainType::GRASS;

            cells[row][col].state =
                CellState::NORMAL;
        }
    }

    startRow = 1;
    startCol = 1;

    targetRow = rows - 2;
    targetCol = cols - 2;

    cells[startRow][startCol].state =
        CellState::START;

    cells[targetRow][targetCol].state =
        CellState::TARGET;
}

// ============================================================
// RANDOM MAZE
// ============================================================

void Grid::generateRandomMaze(
    unsigned int seed)
{
    std::mt19937 rng(seed);

    // ------------------------------------------------
    // START WITH EVERYTHING AS WALL
    // ------------------------------------------------

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            cells[row][col].terrain =
                TerrainType::WALL;

            cells[row][col].state =
                CellState::NORMAL;
        }
    }

    startRow = 1;
    startCol = 1;

    targetRow = rows - 2;
    targetCol = cols - 2;

    // ------------------------------------------------
    // PATH 1
    // ------------------------------------------------

    int row = startRow;
    int col = startCol;

    cells[row][col].terrain =
        TerrainType::GRASS;

    while (col < targetCol)
    {
        col++;

        cells[row][col].terrain =
            TerrainType::GRASS;
    }

    while (row < targetRow)
    {
        row++;

        cells[row][col].terrain =
            TerrainType::GRASS;
    }

    // ------------------------------------------------
    // PATH 2
    // ------------------------------------------------

    row = startRow;
    col = startCol;

    while (row < targetRow)
    {
        row++;

        cells[row][col].terrain =
            TerrainType::GRASS;
    }

    while (col < targetCol)
    {
        col++;

        cells[row][col].terrain =
            TerrainType::GRASS;
    }

    // ------------------------------------------------
    // PATH 3
    // ------------------------------------------------

    row = startRow;
    col = startCol;

    while (col < 4 && col < targetCol)
    {
        col++;

        cells[row][col].terrain =
            TerrainType::GRASS;
    }

    while (row < targetRow)
    {
        row++;

        cells[row][col].terrain =
            TerrainType::GRASS;
    }

    while (col < targetCol)
    {
        col++;

        cells[row][col].terrain =
            TerrainType::GRASS;
    }

    // ------------------------------------------------
    // ADD A FEW RANDOM OPEN CONNECTIONS
    // ------------------------------------------------

    std::uniform_int_distribution<int> rowDist(
        1,
        rows - 2
    );

    std::uniform_int_distribution<int> colDist(
        1,
        cols - 2
    );

    for (int i = 0; i < 8; i++)
    {
        int r = rowDist(rng);
        int c = colDist(rng);

        if (cells[r][c].terrain !=
            TerrainType::WALL)
        {
            continue;
        }

        int neighbours = 0;

        if (r > 0 &&
            cells[r - 1][c].terrain !=
            TerrainType::WALL)
        {
            neighbours++;
        }

        if (r < rows - 1 &&
            cells[r + 1][c].terrain !=
            TerrainType::WALL)
        {
            neighbours++;
        }

        if (c > 0 &&
            cells[r][c - 1].terrain !=
            TerrainType::WALL)
        {
            neighbours++;
        }

        if (c < cols - 1 &&
            cells[r][c + 1].terrain !=
            TerrainType::WALL)
        {
            neighbours++;
        }

        if (neighbours >= 2)
        {
            cells[r][c].terrain =
                TerrainType::GRASS;
        }
    }

    // ------------------------------------------------
    // ADD ONLY A FEW WEIGHTED CELLS
    // ------------------------------------------------

    std::uniform_int_distribution<int> weightChance(
        0,
        99
    );

    int weightedCells = 0;

    for (int attempts = 0;
        attempts < 100 &&
        weightedCells < 5;
        attempts++)
    {
        int r = rowDist(rng);
        int c = colDist(rng);

        if (cells[r][c].terrain !=
            TerrainType::GRASS)
        {
            continue;
        }

        if ((r == startRow &&
            c == startCol) ||
            (r == targetRow &&
                c == targetCol))
        {
            continue;
        }

        if (weightChance(rng) < 65)
        {
            cells[r][c].terrain =
                TerrainType::MUD;
        }
        else
        {
            cells[r][c].terrain =
                TerrainType::WATER;
        }

        weightedCells++;
    }

    // ------------------------------------------------
    // RESTORE START
    // ------------------------------------------------

    cells[startRow][startCol].terrain =
        TerrainType::GRASS;

    cells[startRow][startCol].state =
        CellState::START;

    // ------------------------------------------------
    // RESTORE TARGET
    // ------------------------------------------------

    cells[targetRow][targetCol].terrain =
        TerrainType::GRASS;

    cells[targetRow][targetCol].state =
        CellState::TARGET;
}

// ============================================================
// GET CELL
// ============================================================

Cell Grid::getCell(
    int row,
    int col) const
{
    if (!isValidCell(row, col))
    {
        return {
            TerrainType::WALL,
            CellState::NORMAL
        };
    }

    return cells[row][col];
}