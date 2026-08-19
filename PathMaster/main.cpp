#include <SDL3/SDL.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "Grid.h"
#include "BFS.h"
#include "Dijkstra.h"
#include "AStar.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;

const int ROWS = 10;
const int COLS = 10;
const int CELL_SIZE = 40;

const int PANEL_X = 450;

struct Button
{
    SDL_FRect rect;
    const char* text;
};

struct RunData
{
    std::vector<std::pair<int, int>> visited;
    std::vector<std::pair<int, int>> path;
    std::string name;
    bool found = false;
    int currentVisited = 0;
    int currentPath = 0;
    bool showingPath = false;
};

enum class AlgorithmType
{
    NONE,
    BFS,
    DIJKSTRA,
    ASTAR,
    ALL
};

static bool inside(
    float x,
    float y,
    const SDL_FRect& r)
{
    return x >= r.x &&
           x <= r.x + r.w &&
           y >= r.y &&
           y <= r.y + r.h;
}

static void drawButton(
    SDL_Renderer* renderer,
    const Button& button,
    bool selected = false)
{
    if (selected)
        SDL_SetRenderDrawColor(renderer, 70, 110, 170, 255);
    else
        SDL_SetRenderDrawColor(renderer, 45, 45, 52, 255);

    SDL_RenderFillRect(renderer, &button.rect);

    SDL_SetRenderDrawColor(renderer, 120, 120, 130, 255);
    SDL_RenderRect(renderer, &button.rect);

    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);

    float textX = button.rect.x + 12.0f;
    float textY = button.rect.y + 13.0f;

    SDL_RenderDebugText(
        renderer,
        textX,
        textY,
        button.text
    );
}

static void drawPanelText(
    SDL_Renderer* renderer,
    float x,
    float y,
    const char* text)
{
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    SDL_RenderDebugText(renderer, x, y, text);
}

static RunData prepareRun(
    Grid& grid,
    AlgorithmType type)
{
    RunData data;

    grid.clearSearch();

    if (type == AlgorithmType::BFS)
    {
        BFS algorithm;

        auto start = std::chrono::high_resolution_clock::now();
        data.found = algorithm.solve(grid);
        auto end = std::chrono::high_resolution_clock::now();

        data.name = "BFS";
        data.visited = algorithm.getVisitedOrder();
        data.path = algorithm.getPath();

        double ms =
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();

        std::cout << "BFS | visited: "
                  << data.visited.size()
                  << " | path: "
                  << data.path.size()
                  << " | time: "
                  << ms
                  << " ms\n";
    }
    else if (type == AlgorithmType::DIJKSTRA)
    {
        Dijkstra algorithm;

        auto start = std::chrono::high_resolution_clock::now();
        data.found = algorithm.solve(grid);
        auto end = std::chrono::high_resolution_clock::now();

        data.name = "Dijkstra";
        data.visited = algorithm.getVisitedOrder();
        data.path = algorithm.getPath();

        double ms =
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();

        std::cout << "Dijkstra | visited: "
                  << data.visited.size()
                  << " | path: "
                  << data.path.size()
                  << " | time: "
                  << ms
                  << " ms\n";
    }
    else if (type == AlgorithmType::ASTAR)
    {
        AStar algorithm;

        auto start = std::chrono::high_resolution_clock::now();
        data.found = algorithm.solve(grid);
        auto end = std::chrono::high_resolution_clock::now();

        data.name = "A*";
        data.visited = algorithm.getVisitedOrder();
        data.path = algorithm.getPath();

        double ms =
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();

        std::cout << "A* | visited: "
                  << data.visited.size()
                  << " | path: "
                  << data.path.size()
                  << " | time: "
                  << ms
                  << " ms\n";
    }

    return data;
}

static void paintCurrent(
    Grid& grid,
    const RunData& run)
{
    grid.clearSearch();

    int visitedCount =
        std::min(
            run.currentVisited,
            static_cast<int>(run.visited.size())
        );

    for (int i = 0; i < visitedCount; i++)
    {
        grid.markVisited(
            run.visited[i].first,
            run.visited[i].second
        );
    }

    if (run.showingPath)
    {
        int pathCount =
            std::min(
                run.currentPath,
                static_cast<int>(run.path.size())
            );

        for (int i = 0; i < pathCount; i++)
        {
            grid.markPath(
                run.path[i].first,
                run.path[i].second
            );
        }
    }
}

int main(int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "SDL initialization failed: "
                  << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window =
        SDL_CreateWindow(
            "Path Master",
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            0
        );

    if (!window)
    {
        std::cout << "Window creation failed: "
                  << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, nullptr);

    if (!renderer)
    {
        std::cout << "Renderer creation failed: "
                  << SDL_GetError() << "\n";

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Grid grid(
        ROWS,
        COLS,
        CELL_SIZE
    );

    std::mt19937 rng(
        static_cast<unsigned int>(
            std::chrono::high_resolution_clock::
                now()
                .time_since_epoch()
                .count()
        )
    );

    Button bfsButton{
        {450, 80, 150, 42},
        "BFS"
    };

    Button dijkstraButton{
        {620, 80, 150, 42},
        "DIJKSTRA"
    };

    Button astarButton{
        {790, 80, 150, 42},
        "A STAR"
    };

    Button allButton{
        {450, 135, 150, 42},
        "RUN ALL"
    };

    Button newMazeButton{
        {620, 135, 150, 42},
        "NEW MAZE"
    };

    Button clearButton{
        {790, 135, 150, 42},
        "CLEAR"
    };

    Button slowerButton{
        {450, 190, 100, 38},
        "SLOWER"
    };

    Button fasterButton{
        {560, 190, 100, 38},
        "FASTER"
    };

    Button resetButton{
        {670, 190, 100, 38},
        "RESET"
    };

    bool running = true;
    bool animating = false;

    AlgorithmType selected =
        AlgorithmType::NONE;

    RunData run;

    int speedMs = 45;

    Uint64 lastStep =
        SDL_GetTicks();

    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                float mouseX =
                    event.button.x;

                float mouseY =
                    event.button.y;

                // -------------------------
                // UI
                // -------------------------

                if (inside(mouseX, mouseY, bfsButton.rect))
                {
                    selected = AlgorithmType::BFS;
                    run = prepareRun(grid, selected);
                    run.currentVisited = 0;
                    run.currentPath = 0;
                    run.showingPath = false;
                    animating = true;
                    lastStep = SDL_GetTicks();
                }
                else if (inside(mouseX, mouseY, dijkstraButton.rect))
                {
                    selected = AlgorithmType::DIJKSTRA;
                    run = prepareRun(grid, selected);
                    run.currentVisited = 0;
                    run.currentPath = 0;
                    run.showingPath = false;
                    animating = true;
                    lastStep = SDL_GetTicks();
                }
                else if (inside(mouseX, mouseY, astarButton.rect))
                {
                    selected = AlgorithmType::ASTAR;
                    run = prepareRun(grid, selected);
                    run.currentVisited = 0;
                    run.currentPath = 0;
                    run.showingPath = false;
                    animating = true;
                    lastStep = SDL_GetTicks();
                }
                else if (inside(mouseX, mouseY, allButton.rect))
                {
                    // Run All is handled as a simple sequential
                    // console comparison for now. The selected
                    // algorithm can still be animated normally.
                    std::cout << "\n===== COMPARISON =====\n";

                    grid.clearSearch();

                    RunData b =
                        prepareRun(grid, AlgorithmType::BFS);

                    RunData d =
                        prepareRun(grid, AlgorithmType::DIJKSTRA);

                    RunData a =
                        prepareRun(grid, AlgorithmType::ASTAR);

                    std::cout
                        << "BFS visited: "
                        << b.visited.size()
                        << "\n";

                    std::cout
                        << "Dijkstra visited: "
                        << d.visited.size()
                        << "\n";

                    std::cout
                        << "A* visited: "
                        << a.visited.size()
                        << "\n";

                    std::cout << "======================\n";

                    selected = AlgorithmType::NONE;
                    animating = false;
                }
                else if (inside(mouseX, mouseY, newMazeButton.rect))
                {
                    grid.generateRandomMaze(rng());
                    selected = AlgorithmType::NONE;
                    animating = false;
                }
                else if (inside(mouseX, mouseY, clearButton.rect))
                {
                    grid.clearSearch();
                    selected = AlgorithmType::NONE;
                    animating = false;
                }
                else if (inside(mouseX, mouseY, slowerButton.rect))
                {
                    speedMs =
                        std::min(200, speedMs + 10);
                }
                else if (inside(mouseX, mouseY, fasterButton.rect))
                {
                    speedMs =
                        std::max(5, speedMs - 10);
                }
                else if (inside(mouseX, mouseY, resetButton.rect))
                {
                    grid.clearAll();
                    selected = AlgorithmType::NONE;
                    animating = false;
                }
                else if (!animating)
                {
                    // -------------------------
                    // GRID MOUSE CONTROLS
                    // -------------------------

                    int col =
                        static_cast<int>(mouseX) /
                        CELL_SIZE;

                    int row =
                        static_cast<int>(mouseY) /
                        CELL_SIZE;

                    if (mouseX < ROWS * CELL_SIZE &&
                        row >= 0 &&
                        row < ROWS &&
                        col >= 0 &&
                        col < COLS)
                    {
                        if (event.button.button ==
                            SDL_BUTTON_LEFT)
                        {
                            grid.handleLeftClick(
                                row,
                                col
                            );
                        }
                        else if (event.button.button ==
                                 SDL_BUTTON_RIGHT)
                        {
                            SDL_Keymod modifiers =
                                SDL_GetModState();

                            bool shift =
                                (modifiers &
                                 SDL_KMOD_SHIFT) != 0;

                            grid.handleRightClick(
                                row,
                                col,
                                shift
                            );
                        }
                    }
                }
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_N)
                {
                    grid.generateRandomMaze(rng());
                    selected = AlgorithmType::NONE;
                    animating = false;
                }

                if (event.key.key == SDLK_R)
                {
                    grid.clearSearch();
                    selected = AlgorithmType::NONE;
                    animating = false;
                }

                if (event.key.key == SDLK_C)
                {
                    grid.clearAll();
                    selected = AlgorithmType::NONE;
                    animating = false;
                }
            }
        }

        // -------------------------
        // ANIMATION
        // -------------------------

        if (animating)
        {
            Uint64 now =
                SDL_GetTicks();

            if (now - lastStep >=
                static_cast<Uint64>(speedMs))
            {
                lastStep = now;

                if (!run.showingPath)
                {
                    run.currentVisited++;

                    if (run.currentVisited >=
                        static_cast<int>(
                            run.visited.size()
                        ))
                    {
                        run.showingPath = true;
                        run.currentPath = 0;
                    }
                }
                else
                {
                    run.currentPath++;

                    if (run.currentPath >=
                        static_cast<int>(
                            run.path.size()
                        ))
                    {
                        run.currentPath =
                            static_cast<int>(
                                run.path.size()
                            );

                        animating = false;
                    }
                }

                paintCurrent(grid, run);
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

        // Panel
        SDL_FRect panel{
            static_cast<float>(PANEL_X),
            0.0f,
            static_cast<float>(
                WINDOW_WIDTH - PANEL_X
            ),
            static_cast<float>(WINDOW_HEIGHT)
        };

        SDL_SetRenderDrawColor(
            renderer,
            25,
            25,
            30,
            255
        );

        SDL_RenderFillRect(
            renderer,
            &panel
        );

        drawPanelText(
            renderer,
            470,
            30,
            "PATH MASTER"
        );

        drawPanelText(
            renderer,
            470,
            52,
            "Pathfinding Visualizer"
        );

        drawButton(
            renderer,
            bfsButton,
            selected == AlgorithmType::BFS
        );

        drawButton(
            renderer,
            dijkstraButton,
            selected == AlgorithmType::DIJKSTRA
        );

        drawButton(
            renderer,
            astarButton,
            selected == AlgorithmType::ASTAR
        );

        drawButton(renderer, allButton);
        drawButton(renderer, newMazeButton);
        drawButton(renderer, clearButton);

        drawButton(renderer, slowerButton);
        drawButton(renderer, fasterButton);
        drawButton(renderer, resetButton);

        drawPanelText(
            renderer,
            450,
            255,
            "Controls:"
        );

        drawPanelText(
            renderer,
            450,
            278,
            "Left click = wall"
        );

        drawPanelText(
            renderer,
            450,
            298,
            "Right click = start"
        );

        drawPanelText(
            renderer,
            450,
            318,
            "Shift + right = end"
        );

        drawPanelText(
            renderer,
            450,
            350,
            "Legend:"
        );

        drawPanelText(
            renderer,
            450,
            372,
            "Green circle = Start"
        );

        drawPanelText(
            renderer,
            450,
            392,
            "Red circle = End"
        );

        drawPanelText(
            renderer,
            450,
            412,
            "5 / 10 = terrain cost"
        );

        drawPanelText(
            renderer,
            450,
            432,
            "Blue = visited"
        );

        drawPanelText(
            renderer,
            450,
            452,
            "Yellow = final path"
        );

        drawPanelText(
            renderer,
            450,
            490,
            "Speed:"
        );

        std::string speedText =
            std::to_string(speedMs) +
            " ms";

        drawPanelText(
            renderer,
            510,
            490,
            speedText.c_str()
        );

        if (!run.name.empty())
        {
            drawPanelText(
                renderer,
                450,
                530,
                run.name.c_str()
            );

            std::string visited =
                "Visited: " +
                std::to_string(
                    run.currentVisited
                );

            drawPanelText(
                renderer,
                450,
                550,
                visited.c_str()
            );

            std::string path =
                "Path: " +
                std::to_string(
                    run.currentPath
                );

            drawPanelText(
                renderer,
                450,
                570,
                path.c_str()
            );
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(8);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
