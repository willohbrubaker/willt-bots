#include "simulation.h"
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    Simulation sim(20, 15, 40); // width, height cells, 40 pixels each → ~800x600 window

    if (!sim.init())
    {
        std::cerr << "Failed to initialize simulation\n";
        return 1;
    }

    std::cout << "WilltBot simulation running...\n";
    sim.run();

    return 0;
}