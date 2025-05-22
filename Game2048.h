#ifndef GAME2048_H
#define GAME2048_H

#include <SFML/Graphics.hpp>

enum class GameState {
    MENU,
    GAME
};

class Game2048 {
public:
    Game2048();
    void run();  // 启动游戏主循环

private:
    void processEvents();
    void update();
    void render();
    void drawGrid();
    void drawMenu();

    sf::RenderWindow window;
    sf::Font font;
    sf::Text menuText;

    GameState state;
};

#endif // GAME2048_H
