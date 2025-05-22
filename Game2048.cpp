#include "Game2048.h"
#include <iostream>

const int SIZE = 4;
const int TILE_SIZE = 100;
const int TILE_MARGIN = 10;
const int WINDOW_SIZE = SIZE * (TILE_SIZE + TILE_MARGIN) + TILE_MARGIN;

Game2048::Game2048()
    : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "2048 Game - SFML"),
      state(GameState::MENU)
{
    window.setFramerateLimit(60);

    // font.loadFromFile("/mnt/d/oop/Tasks/final/my2048/arial.ttf");

    // if (!font.loadFromFile("arial.ttf")) {
    //     if (!font.loadFromFile("/mnt/d/oop/Tasks/final/my2048/arial.ttf")) {
    //         // 字体加载失败
    //         throw std::runtime_error("Failed to load font");
    //     }
    // }

    std::vector<std::string> fontPaths = {
        "arial.ttf",                            // 当前目录
        "../arial.ttf",                         // 上级目录（适用于build/结构）
        "../../arial.ttf",                      // 项目根目录
        "resources/arial.ttf",                  // 常见资源目录
        "/mnt/d/oop/Tasks/final/my2048/arial.ttf",  // 你的原始路径
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", // Linux
        "C:/Windows/Fonts/arial.ttf",           // Windows系统字体
        "C:/Windows/Fonts/arialuni.ttf",        // Windows Unicode字体
        "/System/Library/Fonts/Arial.ttf"       // macOS系统字体
    };

    // 2. 尝试所有路径直到成功
    bool fontLoaded = false;
    for (const auto& path : fontPaths) {
        if (font.loadFromFile(path)) {
            fontLoaded = true;
            std::cout << "Successfully loaded font from: " << path << std::endl;
            break;
        }
    }

    // 3. 全部失败后使用SFML默认字体
    if (!fontLoaded) {
        throw std::runtime_error(
                "Failed to load font (place arial.ttf in the executable directory)"
            );
    }

    menuText.setFont(font);
    menuText.setCharacterSize(24);
    menuText.setString("Welcome to 2048!\nPress Enter to start");
    menuText.setFillColor(sf::Color::Black);
    menuText.setPosition(40, WINDOW_SIZE / 2 - 50);
}

void Game2048::run()
{
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game2048::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (state == GameState::MENU && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter)
                state = GameState::GAME;
        }
    }
}

void Game2048::update()
{
    // 游戏逻辑（后续可以写移动和合并）
}

void Game2048::render()
{
    window.clear(sf::Color(250, 248, 239));
    if (state == GameState::MENU)
        drawMenu();
    else if (state == GameState::GAME)
        drawGrid();
    window.display();
}

void Game2048::drawMenu()
{
    window.draw(menuText);
}

void Game2048::drawGrid()
{
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setFillColor(sf::Color(205, 193, 180));
            tile.setPosition(
                TILE_MARGIN + col * (TILE_SIZE + TILE_MARGIN),
                TILE_MARGIN + row * (TILE_SIZE + TILE_MARGIN)
            );
            window.draw(tile);
        }
    }
}
