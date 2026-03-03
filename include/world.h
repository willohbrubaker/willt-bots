#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <iostream>

class World
{
public:
    World(int width, int height);
    void display() const;

    int getWidth() const { return static_cast<int>(grid_[0].size()); }
    int getHeight() const { return static_cast<int>(grid_.size()); }

    char getCell(int x, int y) const
    {
        if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight())
        {
            return '.';
        }
        return grid_[y][x];
    }

    // future setter if we want dynamic world changes (e.g., obstacles)
    // void setCell(int x, int y, char value);

private:
    std::vector<std::vector<char>> grid_; // '.' = empty, 'C' = charging zone
    void initializeGrid();
};

#endif // WORLD_H