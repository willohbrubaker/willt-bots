#ifndef BOT_H
#define BOT_H

#include <SDL2/SDL.h>

#include "world.h"
#include <random>
#include <cmath> 

struct Genome
{
    float speed = 1.0f;
    float turn_rate = 0.1f;
    float sensor_range = 1.0f;
};

class Bot
{
public:
    Bot(float start_x, float start_y, float start_heading, const Genome &genes);

    void update(float dt, const World &world);
    void render(SDL_Renderer *renderer, int cell_size) const;

    float getFitness() const { return energy_; } 

private:
    float x_, y_;
    float heading_;         // Radians
    Genome genome_;
    float energy_ = 100.0f;

    std::mt19937 rng_;

    bool isCharging(const World &world) const;
    void think();
};

#endif // BOT_H