#include "simulation.h"
#include <iostream>
#include <random>
#include <cmath>

Simulation::Simulation(int worldWidth, int worldHeight, int cellSize, int initialBotCount)
    : world_(std::make_unique<World>(worldWidth, worldHeight)),
      cellSize_(cellSize),
      rng_(std::random_device{}())
{
    spawnInitialBots(initialBotCount);
}

Simulation::~Simulation()
{
    cleanup();
}

bool Simulation::init()
{
    if (initialized_)
    {
        std::cout << "Warning: init() called multiple times - skipping\n";
        return true; // Already good
    }

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
                               SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE); // Optional: resizable for polish

    if (!window_)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window_);
        SDL_Quit();
        return false;
    }

    initialized_ = true;
    std::cout << "Simulation window initialized (" << winWidth << "x" << winHeight << ")\n";
    std::cout << "Spawned " << bots_.size() << " WilltBots\n";
    return true;
}

void Simulation::run()
{
    if (!init())
    {
        std::cerr << "Initialization failed - exiting\n";
        return;
    }

    Uint32 lastTime = SDL_GetTicks();
    accumulator_ = 0.0f;

    while (running_)
    {
        handleEvents();

        Uint32 currentTime = SDL_GetTicks();
        float frameTime = static_cast<float>(currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        accumulator_ += frameTime;

        while (accumulator_ >= fixedTimestep_)
        {
            updateSimulation(fixedTimestep_);
            accumulator_ -= fixedTimestep_;
        }

        render();

        SDL_Delay(2);
    }

    cleanup();
}

void Simulation::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            running_ = false;
            break;

            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE) running_ = false;
                break;

        default:
            break;
        }
    }
}

void Simulation::updateSimulation(float dt)
{
    for (auto &bot : bots_)
    {
        bot->update(dt, *world_);
    }
}

void Simulation::render()
{
    SDL_SetRenderDrawColor(renderer_, 18, 18, 28, 255);
    SDL_RenderClear(renderer_);

    int w = world_->getWidth();
    int h = world_->getHeight();

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            char cell = world_->getCell(x, y);

            SDL_Rect rect{x * cellSize_, y * cellSize_, cellSize_, cellSize_};

            if (cell == 'C')
            {
                SDL_SetRenderDrawColor(renderer_, 40, 220, 120, 255); // nicer green
            }
            else
            {
                SDL_SetRenderDrawColor(renderer_, 50, 50, 70, 255);
            }

            SDL_RenderFillRect(renderer_, &rect);

            SDL_SetRenderDrawColor(renderer_, 35, 35, 45, 255);
            SDL_RenderDrawRect(renderer_, &rect);
        }
    }

    for (const auto &bot : bots_)
    {
        bot->render(renderer_, cellSize_);
    }

    SDL_RenderPresent(renderer_);
}

void Simulation::cleanup()
{
    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_Quit();
    initialized_ = false; 
}

void Simulation::spawnInitialBots(int count)
{
    std::uniform_real_distribution<float> x_dist(0.0f, static_cast<float>(world_->getWidth()));
    std::uniform_real_distribution<float> y_dist(0.0f, static_cast<float>(world_->getHeight()));
    std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * static_cast<float>(M_PI));

    bots_.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        Genome baseGenome;

        auto bot = std::make_unique<Bot>(
            x_dist(rng_),
            y_dist(rng_),
            angle_dist(rng_),
            baseGenome);

        bots_.push_back(std::move(bot));
    }
}