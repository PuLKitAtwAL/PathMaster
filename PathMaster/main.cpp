#include <SDL3/SDL.h>

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>

#include "Grid.h"
#include "BFS.h"
#include "Dijkstra.h"
#include "AStar.h"
#include "Benchmark.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;

const int ROWS = 10;
const int COLS = 10;
const int CELL_SIZE = 40;

void showResult(
    const BenchmarkResult& result)
{
    std::cout << "\n============================\n";

    std::cout << result.algorithm
        << "\n";

    std::cout << "Path found: "
        << (result.foundPath ? "YES" : "NO")
        << "\n";

    std::cout << "Visited nodes: "
        << result.visitedNodes
        << "\n";

    std::cout << "Path length: "
        << result.pathLength
        << "\n";

    std::cout << "Execution time: "
        << result.executionTimeMs
        << " ms\n";

    std::cout << "============================\n";
}

int main(int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout
            << "SDL initialization failed: "
            << SDL_GetError()
            << "\n";

        return 1;
    }

    SDL_Window* window =
        SDL_CreateWindow(
            "Path Master",
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            0
        );

    if (window == nullptr)
    {
        std::cout
            << "Window creation failed: "
            << SDL_GetError()
            << "\n";

        SDL_Quit();

        return 1;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(
            window,
            nullptr
        );

    if (renderer == nullptr)
    {
        std::cout
            << "Renderer creation failed: "
            << SDL_GetError()
            << "\n";

        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    Grid grid(
        ROWS,
        COLS,
        CELL_SIZE
    );

    // Random number generator
    std::mt19937 rng(
        static_cast<unsigned int>(
            std::chrono::high_resolution_clock::
            now()
            .time_since_epoch()
            .count()
            )
    );

    bool running = true;

    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            // -------------------------
            // QUIT
            // -------------------------

            if (event.type ==
                SDL_EVENT_QUIT)
            {
                running = false;
            }

            // -------------------------
            // MOUSE
            // -------------------------

            if (event.type ==
                SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                int mouseX =
                    static_cast<int>(
                        event.button.x
                        );

                int mouseY =
                    static_cast<int>(
                        event.button.y
                        );

                int col =
                    mouseX / CELL_SIZE;

                int row =
                    mouseY / CELL_SIZE;

                // Make sure click is
                // actually inside grid
                if (row >= 0 &&
                    row < ROWS &&
                    col >= 0 &&
                    col < COLS)
                {
                    // LEFT CLICK = WALL
                    if (event.button.button ==
                        SDL_BUTTON_LEFT)
                    {
                        grid.handleLeftClick(
                            row,
                            col
                        );
                    }

                    // RIGHT CLICK
                    if (event.button.button ==
                        SDL_BUTTON_RIGHT)
                    {
                        SDL_Keymod modifiers =
                            SDL_GetModState();

                        bool shiftPressed =
                            (modifiers &
                                SDL_KMOD_SHIFT) != 0;

                        grid.handleRightClick(
                            row,
                            col,
                            shiftPressed
                        );
                    }
                }
            }

            // -------------------------
            // KEYBOARD
            // -------------------------

            if (event.type ==
                SDL_EVENT_KEY_DOWN)
            {
                // =====================
                // SPACE = BFS
                // =====================

                if (event.key.key ==
                    SDLK_SPACE)
                {
                    grid.clearSearch();

                    BFS bfs;

                    bool found =
                        bfs.solve(grid);

                    for (const auto& cell :
                        bfs.getVisitedOrder())
                    {
                        grid.markVisited(
                            cell.first,
                            cell.second
                        );
                    }

                    for (const auto& cell :
                        bfs.getPath())
                    {
                        grid.markPath(
                            cell.first,
                            cell.second
                        );
                    }

                    BenchmarkResult result =
                        Benchmark::runBFS(
                            grid
                        );

                    showResult(result);

                    std::cout
                        << "BFS path found: "
                        << (found ? "YES" : "NO")
                        << "\n";
                }

                // =====================
                // D = DIJKSTRA
                // =====================

                if (event.key.key ==
                    SDLK_D)
                {
                    grid.clearSearch();

                    Dijkstra algorithm;

                    bool found =
                        algorithm.solve(grid);

                    for (const auto& cell :
                        algorithm.getVisitedOrder())
                    {
                        grid.markVisited(
                            cell.first,
                            cell.second
                        );
                    }

                    for (const auto& cell :
                        algorithm.getPath())
                    {
                        grid.markPath(
                            cell.first,
                            cell.second
                        );
                    }

                    BenchmarkResult result =
                        Benchmark::runDijkstra(
                            grid
                        );

                    showResult(result);

                    std::cout
                        << "Dijkstra path found: "
                        << (found ? "YES" : "NO")
                        << "\n";
                }

                // =====================
                // A = A*
                // =====================

                if (event.key.key ==
                    SDLK_A)
                {
                    grid.clearSearch();

                    AStar algorithm;

                    bool found =
                        algorithm.solve(grid);

                    for (const auto& cell :
                        algorithm.getVisitedOrder())
                    {
                        grid.markVisited(
                            cell.first,
                            cell.second
                        );
                    }

                    for (const auto& cell :
                        algorithm.getPath())
                    {
                        grid.markPath(
                            cell.first,
                            cell.second
                        );
                    }

                    BenchmarkResult result =
                        Benchmark::runAStar(
                            grid
                        );

                    showResult(result);

                    std::cout
                        << "A* path found: "
                        << (found ? "YES" : "NO")
                        << "\n";
                }

                // =====================
                // R = CLEAR SEARCH
                // =====================

                if (event.key.key ==
                    SDLK_R)
                {
                    grid.clearSearch();

                    std::cout
                        << "Search cleared.\n";
                }

                // =====================
                // C = COMPLETE RESET
                // =====================

                if (event.key.key ==
                    SDLK_C)
                {
                    grid.clearAll();

                    std::cout
                        << "Grid reset.\n";
                }

                // =====================
                // N = NEW RANDOM MAZE
                // =====================

                if (event.key.key ==
                    SDLK_N)
                {
                    unsigned int seed =
                        rng();

                    grid.generateRandomMaze(
                        seed
                    );

                    std::cout
                        << "\nNew random maze generated.\n";

                    std::cout
                        << "Seed: "
                        << seed
                        << "\n";
                }
            }
        }

        // -------------------------
        // DRAW
        // -------------------------

        SDL_SetRenderDrawColor(
            renderer,
            15,
            15,
            20,
            255
        );

        SDL_RenderClear(renderer);

        grid.draw(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}