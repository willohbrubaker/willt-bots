#include "simulation.h"
#include <iostream>

Simulation::Simulation(int worldWidth, int worldHeight, int cellSize)
    : world_(std::make_unique<World>(worldWidth, worldHeight)),
      cellSize_(cellSize) {}

Simulation::~Simulation()
{
    cleanup();
}

bool Simulation::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    int winWidth = world_->getWidth() * cellSize_;
    int winHeight = world_->getHeight() * cellSize_;

    window_ = SDL_CreateWindow("WilltBot AMR Simulation",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               winWidth, winHeight,
                               SDL_WINDOW_SHOWN);
    if (!window_)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        return false;
    }

    std::cout << "Simulation window initialized (" << winWidth << "x" << winHeight << ")\n";
    return true;
}

void Simulation::run()
{
    if (!init())
        return;

    Uint32 lastTime = SDL_GetTicks();
    while (running_)
    {
        handleEvents();

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastTime >= 100)
        { // ~10 FPS for now – low CPU
            render();
            lastTime = currentTime;
        }

        SDL_Delay(10); // Prevent 100% CPU
    }
}

void Simulation::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            running_ = false;
        }
        // Later: key/mouse for pausing, zooming, etc.
    }
}

void Simulation::render()
{
    SDL_SetRenderDrawColor(renderer_, 20, 20, 30, 255); // Dark background
    SDL_RenderClear(renderer_);

    int w = world_->getWidth();
    int h = world_->getHeight();

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            char cell = world_->getCell(x, y); // We'll add this getter

            SDL_Rect rect = {x * cellSize_, y * cellSize_, cellSize_, cellSize_};

            if (cell == 'C')
            {
                SDL_SetRenderDrawColor(renderer_, 0, 220, 100, 255); // Bright green charging
            }
            else
            {
                SDL_SetRenderDrawColor(renderer_, 60, 60, 80, 255); // Dim empty
            }

            SDL_RenderFillRect(renderer_, &rect);

            // Optional border for grid look
            SDL_SetRenderDrawColor(renderer_, 30, 30, 40, 255);
            SDL_RenderDrawRect(renderer_, &rect);
        }
    }

    SDL_RenderPresent(renderer_);
}

void Simulation::cleanup()
{
    if (renderer_)
        SDL_DestroyRenderer(renderer_);
    if (window_)
        SDL_DestroyWindow(window_);
    SDL_Quit();
}