#ifndef SIMULATION_H
#define SIMULATION_H

#include "world.h"
#include <SDL2/SDL.h>
#include <memory>

class Simulation
{
public:
    Simulation(int worldWidth, int worldHeight, int cellSize = 40);
    ~Simulation();

    bool init();
    void run();
    void render();

private:
    std::unique_ptr<World> world_;
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;

    int cellSize_;
    bool running_ = true;

    void handleEvents();
    void cleanup();
};

#endif // SIMULATION_H