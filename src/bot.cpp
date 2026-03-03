#include "bot.h"

Bot::Bot(float start_x, float start_y, float start_heading, const Genome &genes)
    : x_(start_x), y_(start_y), heading_(start_heading), genome_(genes),
      rng_(std::random_device{}()) {}

void Bot::update(float dt, const World &world)
{
    // Drain energy over time
    energy_ -= 10.0f * dt;
    if (energy_ < 0.0f)
        energy_ = 0.0f;

    // Think: for now, random turn
    think();

    // Move forward
    x_ += genome_.speed * std::cos(heading_) * dt;
    y_ += genome_.speed * std::sin(heading_) * dt;

    // Wrap around world edges (toroidal world for simplicity)
    x_ = std::fmod(x_ + world.getWidth(), world.getWidth());
    y_ = std::fmod(y_ + world.getHeight(), world.getHeight());

    if (isCharging(world))
    {
        energy_ += 50.0f * dt;
        if (energy_ > 200.0f)
            energy_ = 200.0f;
    }
}

bool Bot::isCharging(const World &world) const
{
    int cell_x = static_cast<int>(x_);
    int cell_y = static_cast<int>(y_);
    return world.getCell(cell_x, cell_y) == 'C';
}

void Bot::think()
{
    std::uniform_real_distribution<float> dist(-genome_.turn_rate, genome_.turn_rate);
    heading_ += dist(rng_);
    heading_ = std::fmod(heading_, 2.0f * M_PI);
    }

void Bot::render(SDL_Renderer *renderer, int cell_size) const
{
    Uint8 r = static_cast<Uint8>(255 * (1.0f - energy_ / 200.0f)); // Low energy: red
    Uint8 g = static_cast<Uint8>(255 * (energy_ / 200.0f));        // High: green
    Uint8 b = 0;

    if (energy_ > 150.0f)
    {
        b = 100;
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    int radius = cell_size / 4;
    int cx = static_cast<int>(x_ * cell_size) + cell_size / 2;
    int cy = static_cast<int>(y_ * cell_size) + cell_size / 2;

    for (int dy = -radius; dy <= radius; ++dy)
    {
        for (int dx = -radius; dx <= radius; ++dx)
        {
            if (dx * dx + dy * dy <= radius * radius)
            {
                SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
            }
        }
    }
}