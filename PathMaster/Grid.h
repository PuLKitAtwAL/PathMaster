#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <utility>

enum class CellType
{
    EMPTY,
    WALL,
    START,
    TARGET,
    VISITED,
    PATH
};

class Grid
{
private:
    int rows;
    int cols;
    int cellSize;

    std::vector<std::vector<CellType>> cells;

    int startRow;
    int startCol;

    int targetRow;
    int targetCol;

public:
    Grid(int rows, int cols, int cellSize);

    void draw(SDL_Renderer* renderer);

    void handleLeftClick(int row, int col);
    void handleRightClick(int row, int col, bool shiftPressed);

    bool isValidCell(int row, int col) const;
    bool isWall(int row, int col) const;

    int getRows() const;
    int getCols() const;

    int getStartRow() const;
    int getStartCol() const;

    int getTargetRow() const;
    int getTargetCol() const;

    void clearSearch();

    void markVisited(int row, int col);
    void markPath(int row, int col);

    void setCell(int row, int col, CellType type);
    CellType getCell(int row, int col) const;
};