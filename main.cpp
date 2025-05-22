#include "Game2048.h"
#include <iostream>

int main()
{
    try {
        Game2048 game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
