#pragma once

#include <SDL3/SDL.h>
#include <vector>

enum class TerrainType
{
    GRASS,
    MUD,
    WATER,
    WALL
};

enum class CellState
{
    NORMAL,
    START,
    TARGET,
    VISITED,
    PATH
};

struct Cell
{
    TerrainType terrain;
    CellState state;
};

class Grid
{
private:
    int rows;
    int cols;
    int cellSize;

    std::vector<std::vector<Cell>> cells;

    int startRow;
    int startCol;

    int targetRow;
    int targetCol;

    void drawCircle(
        SDL_Renderer* renderer,
        float centerX,
        float centerY,
        float radius
    );

    void drawDigit(
        SDL_Renderer* renderer,
        int digit,
        int x,
        int y,
        int size
    );

    void drawNumber(
        SDL_Renderer* renderer,
        int number,
        int x,
        int y
    );

public:
    Grid(int rows, int cols, int cellSize);

    void draw(SDL_Renderer* renderer);

    void handleLeftClick(int row, int col);

    void handleRightClick(
        int row,
        int col,
        bool shiftPressed
    );

    bool isValidCell(int row, int col) const;

    bool isWall(int row, int col) const;

    int getRows() const;
    int getCols() const;

    int getStartRow() const;
    int getStartCol() const;

    int getTargetRow() const;
    int getTargetCol() const;

    int getWeight(int row, int col) const;

    TerrainType getTerrain(
        int row,
        int col
    ) const;

    void setTerrain(
        int row,
        int col,
        TerrainType terrain
    );

    void clearSearch();

    void markVisited(int row, int col);

    void markPath(int row, int col);

    void clearAll();

    void generateRandomMaze(
        unsigned int seed
    );

    Cell getCell(
        int row,
        int col
    ) const;
};