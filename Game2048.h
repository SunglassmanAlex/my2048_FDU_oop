#ifndef GAME2048_H
#define GAME2048_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <algorithm>

enum class GameState {
    MAIN_MENU,
    VERSION_MENU,
    GAME,
    EXIT_CONFIRM,
};

enum class GameVersion {
    ORIGINAL,
    MODIFIED
};

class Game {
public:
    Game();
    void run();

private:
    // Window and state
    sf::RenderWindow window;
    GameState currentState;
    GameVersion currentVersion;
    int gridSize;

    // 在原有动画相关成员变量旁添加
    struct NewTileAnimation {
        sf::Vector2f position;
        float progress;
    };
    std::vector<NewTileAnimation> newTileAnimations;
    float spawnAnimationDuration = 0.3f; // 新方块生成动画持续时间

    // Game data
    std::vector<std::vector<int>> grid;
    int score;
    bool gameOver;
    bool gameWon;
    
    // Resources
    sf::Font font;
    sf::Texture tileTexture;
    std::array<sf::Color, 12> tileColors;
    
    // UI Elements - Main Menu
    sf::Text titleText;
    std::array<sf::RectangleShape, 3> sizeButtons;
    std::array<sf::Text, 3> sizeButtonTexts;
    
    // UI Elements - Version Menu
    sf::Text versionTitleText;
    std::array<sf::RectangleShape, 2> versionButtons;
    std::array<sf::Text, 2> versionButtonTexts;
    
    // Game UI
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text restartText;

    // Exit
    sf::RectangleShape exitConfirmBackground;
    sf::RectangleShape exitConfirmBox;
    sf::Text exitConfirmText;
    sf::RectangleShape exitConfirmYesButton;
    sf::RectangleShape exitConfirmNoButton;
    sf::Text exitConfirmYesText;
    sf::Text exitConfirmNoText;

    void setupExitConfirmUI();  

    // Draw black and white grids in the modified version
    sf::Color getCellBackgroundColor(int x, int y) const;
    
    // Core functions
    void processEvents();
    void update();
    void render();
    
    // State rendering
    void renderMainMenu();
    void renderVersionMenu();
    void renderGame();

    void calculateGridLayout();
    sf::Vector2f getTilePosition(int x, int y) const;
    
    // Input handling
    void handleMainMenuClick(const sf::Vector2f& mousePos);
    void handleVersionMenuClick(const sf::Vector2f& mousePos);
    void handleGameInput(sf::Keyboard::Key key);
    
    // Game logic
    void initializeGame(int size, GameVersion version);
    void resetGame();
    void addRandomTile();
    bool moveTiles(int dx, int dy);
    bool moveTilesContinuous(int dx, int dy);
    bool isGameOver() const;
    bool isGameOver_grid() const;
    bool isGameOVer_diagonal() const;
    
    // Helper functions
    void initializeUI();
    void setupMainMenu();
    void setupVersionMenu();
    void setupTileColors();
    sf::Color getTileColor(int value) const;
    void drawGrid();
};

#endif // GAME2048_H