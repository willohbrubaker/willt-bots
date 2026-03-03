#include "world.h"

World::World(int width, int height)
    : grid_(height, std::vector<char>(width, '.'))
{
    initializeGrid();
}

void World::initializeGrid()
{
    grid_[2][3] = 'C';
    grid_[5][7] = 'C';
    grid_[8][1] = 'C';
}

void World::display() const
{
    std::cout << "WilltBot World (" << getWidth() << "x" << getHeight() << "):\n";
    for (const auto &row : grid_)
    {
        for (char cell : row)
        {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }
}