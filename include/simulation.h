#ifndef SIMULATION_H
#define SIMULATION_H

#include "world.h"
#include "bot.h"

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <random>

class Simulation
{
public:
    Simulation(int worldWidth = 20,
               int worldHeight = 15,
               int cellSize = 40,
               int initialBotCount = 10);

    ~Simulation();

    bool init();
    void run(); 

private:
    std::unique_ptr<World> world_;
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;

    int cellSize_;
    bool running_ = true;

    std::vector<std::unique_ptr<Bot>> bots_;
    std::mt19937 rng_;

    float accumulator_ = 0.0f;
    const float fixedTimestep_ = 1.0f / 60.0f;

    bool initialized_ = false;

    // Core loop methods
    void handleEvents();
    void updateSimulation(float dt); // Physics / bot logic tick
    void render();                   // Draw world + bots
    void cleanup();

    void spawnInitialBots(int count);
};

#endif // SIMULATION_H