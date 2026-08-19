#include "Grid.h"

Grid::Grid(int rows, int cols, int cellSize)
{
    this->rows = rows;
    this->cols = cols;
    this->cellSize = cellSize;

    cells = std::vector<std::vector<CellType>>(
        rows,
        std::vector<CellType>(cols, CellType::EMPTY)
    );

    startRow = 2;
    startCol = 2;

    targetRow = rows - 3;
    targetCol = cols - 3;

    cells[startRow][startCol] = CellType::START;
    cells[targetRow][targetCol] = CellType::TARGET;
}

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

    return cells[row][col] == CellType::WALL;
}

void Grid::handleLeftClick(int row, int col)
{
    if (!isValidCell(row, col))
        return;

    if (cells[row][col] == CellType::START ||
        cells[row][col] == CellType::TARGET)
    {
        return;
    }

    if (cells[row][col] == CellType::WALL)
    {
        cells[row][col] = CellType::EMPTY;
    }
    else
    {
        cells[row][col] = CellType::WALL;
    }
}

void Grid::handleRightClick(
    int row,
    int col,
    bool shiftPressed)
{
    if (!isValidCell(row, col))
        return;

    // Shift + Right Click = Target
    if (shiftPressed)
    {
        if (row == startRow && col == startCol)
            return;

        cells[targetRow][targetCol] = CellType::EMPTY;

        targetRow = row;
        targetCol = col;

        cells[targetRow][targetCol] = CellType::TARGET;

        return;
    }

    // Right Click = Start
    if (row == targetRow && col == targetCol)
        return;

    cells[startRow][startCol] = CellType::EMPTY;

    startRow = row;
    startCol = col;

    cells[startRow][startCol] = CellType::START;
}

void Grid::draw(SDL_Renderer* renderer)
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            SDL_FRect cell;

            cell.x = static_cast<float>(col * cellSize);
            cell.y = static_cast<float>(row * cellSize);
            cell.w = static_cast<float>(cellSize);
            cell.h = static_cast<float>(cellSize);

            switch (cells[row][col])
            {
            case CellType::EMPTY:
                SDL_SetRenderDrawColor(
                    renderer, 25, 25, 30, 255
                );
                break;

            case CellType::WALL:
                SDL_SetRenderDrawColor(
                    renderer, 70, 70, 75, 255
                );
                break;

            case CellType::START:
                SDL_SetRenderDrawColor(
                    renderer, 0, 210, 110, 255
                );
                break;

            case CellType::TARGET:
                SDL_SetRenderDrawColor(
                    renderer, 230, 60, 70, 255
                );
                break;

            case CellType::VISITED:
                SDL_SetRenderDrawColor(
                    renderer, 60, 130, 210, 255
                );
                break;

            case CellType::PATH:
                SDL_SetRenderDrawColor(
                    renderer, 255, 210, 50, 255
                );
                break;
            }

            SDL_RenderFillRect(renderer, &cell);

            SDL_SetRenderDrawColor(
                renderer, 90, 90, 95, 255
            );

            SDL_RenderRect(renderer, &cell);
        }
    }
}

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

void Grid::clearSearch()
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (cells[row][col] == CellType::VISITED ||
                cells[row][col] == CellType::PATH)
            {
                cells[row][col] = CellType::EMPTY;
            }
        }
    }

    cells[startRow][startCol] = CellType::START;
    cells[targetRow][targetCol] = CellType::TARGET;
}

void Grid::markVisited(int row, int col)
{
    if (!isValidCell(row, col))
        return;

    if (cells[row][col] == CellType::EMPTY)
    {
        cells[row][col] = CellType::VISITED;
    }
}

void Grid::markPath(int row, int col)
{
    if (!isValidCell(row, col))
        return;

    if (cells[row][col] != CellType::START &&
        cells[row][col] != CellType::TARGET)
    {
        cells[row][col] = CellType::PATH;
    }
}

void Grid::setCell(int row, int col, CellType type)
{
    if (!isValidCell(row, col))
        return;

    cells[row][col] = type;
}

CellType Grid::getCell(int row, int col) const
{
    if (!isValidCell(row, col))
        return CellType::WALL;

    return cells[row][col];
}