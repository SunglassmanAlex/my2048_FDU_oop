#include "Game2048.h"
#include <stdexcept>
#include <random>
#include <sstream>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;
constexpr int GRID_OFFSET_X = 100;
constexpr int GRID_OFFSET_Y = 200;
constexpr int TILE_SIZE = 100;
constexpr int TILE_MARGIN = 10;

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2048 Game"),
               currentState(GameState::MAIN_MENU),
               currentVersion(GameVersion::ORIGINAL),
               gridSize(4),
               score(0),
               gameOver(false),
               gameWon(false) {
    
    if (!font.loadFromFile("../arial.ttf")) {
        throw std::runtime_error("Failed to load font");
    }
    
    setupTileColors();
    initializeUI();
    setupExitConfirmUI();
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::setupTileColors() {
    tileColors = {
        sf::Color(238, 228, 218), // 2
        sf::Color(237, 224, 200), // 4
        sf::Color(242, 177, 121), // 8
        sf::Color(245, 149, 99),  // 16
        sf::Color(246, 124, 95),  // 32
        sf::Color(246, 94, 59),   // 64
        sf::Color(237, 207, 114), // 128
        sf::Color(237, 204, 97),  // 256
        sf::Color(237, 200, 80),  // 512
        sf::Color(237, 197, 63),  // 1024
        sf::Color(237, 194, 46),  // 2048
        sf::Color(60, 58, 50)     // >2048
    };
}

void Game::setupExitConfirmUI() {
    // 半透明背景
    exitConfirmBackground.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    exitConfirmBackground.setFillColor(sf::Color(0, 0, 0, 150));
    
    // 确认框
    exitConfirmBox.setSize(sf::Vector2f(500, 200));
    exitConfirmBox.setPosition(WINDOW_WIDTH/2 - 250, WINDOW_HEIGHT/2 - 100);
    exitConfirmBox.setFillColor(sf::Color(143, 122, 102));
    
    // 确认文字
    exitConfirmText.setFont(font);
    exitConfirmText.setString("Are you sure you want to quit?");
    exitConfirmText.setCharacterSize(28);
    exitConfirmText.setFillColor(sf::Color::White);
    exitConfirmText.setPosition(WINDOW_WIDTH/2 - 180, WINDOW_HEIGHT/2 - 70);
    
    // 是/否按钮
    exitConfirmYesButton.setSize(sf::Vector2f(120, 50));
    exitConfirmYesButton.setPosition(WINDOW_WIDTH/2 - 140, WINDOW_HEIGHT/2 + 20);
    exitConfirmYesButton.setFillColor(sf::Color(100, 255, 100));
    
    exitConfirmNoButton.setSize(sf::Vector2f(120, 50));
    exitConfirmNoButton.setPosition(WINDOW_WIDTH/2 + 20, WINDOW_HEIGHT/2 + 20);
    exitConfirmNoButton.setFillColor(sf::Color(255, 100, 100));
    
    exitConfirmYesText.setFont(font);
    exitConfirmYesText.setString("Yes");
    exitConfirmYesText.setCharacterSize(24);
    exitConfirmYesText.setFillColor(sf::Color::White);
    exitConfirmYesText.setPosition(WINDOW_WIDTH/2 - 110, WINDOW_HEIGHT/2 + 35);
    
    exitConfirmNoText.setFont(font);
    exitConfirmNoText.setString("No");
    exitConfirmNoText.setCharacterSize(24);
    exitConfirmNoText.setFillColor(sf::Color::White);
    exitConfirmNoText.setPosition(WINDOW_WIDTH/2 + 60, WINDOW_HEIGHT/2 + 35);
}

void Game::initializeUI() {
    setupMainMenu();
    setupVersionMenu();
    
    // Game UI elements
    scoreText.setFont(font);
    scoreText.setCharacterSize(32);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, 20);
    
    gameOverText.setFont(font);
    gameOverText.setString("Game Over!");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 - 50);
    
    restartText.setFont(font);
    restartText.setString("Press R to Restart");
    restartText.setCharacterSize(24);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 20);
}

void Game::setupMainMenu() {
    titleText.setFont(font);
    titleText.setString("2048 Game");
    titleText.setCharacterSize(64);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(280, 100);
    
    const std::array<std::string, 3> sizeLabels = {"4 x 4", "5 x 5", "6 x 6"};
    const std::array<sf::Color, 3> buttonColors = {
        sf::Color(143, 122, 102),
        sf::Color(143, 122, 102),
        sf::Color(143, 122, 102)
    };
    
    for (size_t i = 0; i < sizeButtons.size(); ++i) {
        sizeButtons[i].setSize(sf::Vector2f(200, 80));
        sizeButtons[i].setPosition(300, 250 + i * 120);
        sizeButtons[i].setFillColor(buttonColors[i]);
        
        sizeButtonTexts[i].setFont(font);
        sizeButtonTexts[i].setString(sizeLabels[i]);
        sizeButtonTexts[i].setCharacterSize(32);
        sizeButtonTexts[i].setFillColor(sf::Color::White);
        sf::FloatRect textRect = sizeButtonTexts[i].getLocalBounds();
        sizeButtonTexts[i].setOrigin(textRect.left + textRect.width/2.0f,
                                    textRect.top + textRect.height/2.0f);
        sizeButtonTexts[i].setPosition(300 + 100, 250 + i * 120 + 40);
    }
}

void Game::setupVersionMenu() {
    versionTitleText.setFont(font);
    versionTitleText.setString("Choose Version");
    versionTitleText.setCharacterSize(48);
    versionTitleText.setFillColor(sf::Color::White);
    versionTitleText.setPosition(280, 100);
    
    const std::array<std::string, 2> versionLabels = {
        "Original Version",
        "Modified Version"
    };
    
    for (size_t i = 0; i < versionButtons.size(); ++i) {
        versionButtons[i].setSize(sf::Vector2f(400, 80));
        versionButtons[i].setPosition(200, 250 + i * 120);
        versionButtons[i].setFillColor(sf::Color(143, 122, 102));
        
        versionButtonTexts[i].setFont(font);
        versionButtonTexts[i].setString(versionLabels[i]);
        versionButtonTexts[i].setCharacterSize(32);
        versionButtonTexts[i].setFillColor(sf::Color::White);
        sf::FloatRect textRect = versionButtonTexts[i].getLocalBounds();
        versionButtonTexts[i].setOrigin(textRect.left + textRect.width/2.0f,
                                       textRect.top + textRect.height/2.0f);
        versionButtonTexts[i].setPosition(200 + 200, 250 + i * 120 + 40);
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        // 关闭窗口
        if (event.type == sf::Event::Closed) {
            currentState = GameState::EXIT_CONFIRM;
        }

        // 键盘输入处理
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                currentState = GameState::EXIT_CONFIRM;
            }

            // 退出确认界面
            if (currentState == GameState::EXIT_CONFIRM) {
                if (event.key.code == sf::Keyboard::Y) {
                    window.close();
                } else if (event.key.code == sf::Keyboard::N) {
                    if (grid.empty()) {
                        currentState = GameState::MAIN_MENU;
                    } else {
                        currentState = GameState::GAME;
                    }
                }
            }

            // 方向键 和 R 键逻辑：只在游戏中触发
            if (currentState == GameState::GAME) {
                if (event.key.code == sf::Keyboard::R) {
                    resetGame();
                } else if (!gameOver) {
                    handleGameInput(event.key.code);  // 支持方向键
                }
            }
        }

        // 鼠标点击事件
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

            if (currentState == GameState::EXIT_CONFIRM) {
                if (exitConfirmYesButton.getGlobalBounds().contains(mousePos)) {
                    window.close();
                } else if (exitConfirmNoButton.getGlobalBounds().contains(mousePos)) {
                    if (grid.empty()) {
                        currentState = GameState::MAIN_MENU;
                    } else {
                        currentState = GameState::GAME;
                    }
                }
                continue;
            }

            // 菜单点击逻辑
            if (currentState == GameState::MAIN_MENU) {
                handleMainMenuClick(mousePos);
            } else if (currentState == GameState::VERSION_MENU) {
                handleVersionMenuClick(mousePos);
            }
        }
    }
}


void Game::handleMainMenuClick(const sf::Vector2f& pos) {
    for (size_t i = 0; i < sizeButtons.size(); ++i) {
        if (sizeButtons[i].getGlobalBounds().contains(pos)) {
            gridSize = 4 + i;
            currentState = GameState::VERSION_MENU;
            return;
        }
    }
}

void Game::handleVersionMenuClick(const sf::Vector2f& pos) {
    for (size_t i = 0; i < versionButtons.size(); ++i) {
        if (versionButtons[i].getGlobalBounds().contains(pos)) {
            currentVersion = static_cast<GameVersion>(i);
            initializeGame(gridSize, currentVersion);
            currentState = GameState::GAME;
            return;
        }
    }
}

void Game::handleGameInput(sf::Keyboard::Key key) {
    bool moved = false;
    
    switch (key) {
        case sf::Keyboard::Up:    moved = moveTiles(0, -1); break;
        case sf::Keyboard::Down:  moved = moveTiles(0, 1);  break;
        case sf::Keyboard::Left:  moved = moveTiles(-1, 0); break;
        case sf::Keyboard::Right: moved = moveTiles(1, 0);  break;
        default: break;
    }
    
    if (moved) {
        addRandomTile();
        gameOver = isGameOver();
    }
}

void Game::initializeGame(int size, GameVersion version) {
    gridSize = size;
    currentVersion = version;
    resetGame();
}

void Game::resetGame() {
    grid.clear();
    grid.resize(gridSize, std::vector<int>(gridSize, 0));
    score = 0;
    gameOver = false;
    gameWon = false;
    
    // Add initial tiles
    addRandomTile();
    addRandomTile();
}

void Game::addRandomTile() {
    std::vector<std::pair<int, int>> emptyCells;
    
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            if (grid[y][x] == 0) {
                emptyCells.emplace_back(x, y);
            }
        }
    }
    
    if (!emptyCells.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, emptyCells.size() - 1);
        
        auto [x, y] = emptyCells[dist(gen)];
        grid[y][x] = (dist(gen) % 10 < 8) ? 2 : 4;
    }
}

bool Game::moveTiles(int dx, int dy) {
    bool moved = false;
    
    if (dx != 0 && dy != 0) return false;
    
    // Determine iteration order based on direction
    int startX = dx == 1 ? gridSize - 1 : 0;
    int endX = dx == 1 ? -1 : gridSize;
    int stepX = dx == 1 ? -1 : 1;
    
    int startY = dy == 1 ? gridSize - 1 : 0;
    int endY = dy == 1 ? -1 : gridSize;
    int stepY = dy == 1 ? -1 : 1;
    
    for (int y = startY; y != endY; y += stepY) {
        for (int x = startX; x != endX; x += stepX) {
            if (grid[y][x] == 0) continue;
            
            int newX = x;
            int newY = y;
            int currentX = x;
            int currentY = y;
            
            while (true) {
                currentX += dx;
                currentY += dy;
                
                if (currentX < 0 || currentX >= gridSize || 
                    currentY < 0 || currentY >= gridSize) {
                    break;
                }
                
                if (grid[currentY][currentX] == 0) {
                    newX = currentX;
                    newY = currentY;
                } else if (grid[currentY][currentX] == grid[y][x]) {
                    newX = currentX;
                    newY = currentY;
                    break;
                } else {
                    break;
                }
            }
            
            if (newX != x || newY != y) {
                if (grid[newY][newX] == grid[y][x]) {
                    // Merge tiles
                    grid[newY][newX] *= 2;
                    score += grid[newY][newX];
                    if (grid[newY][newX] == 2048) {
                        gameWon = true;
                    }
                } else {
                    // Move tile
                    grid[newY][newX] = grid[y][x];
                }
                
                grid[y][x] = 0;
                moved = true;
            }
        }
    }
    
    return moved;
}

bool Game::isGameOver() const {
    // Check for empty cells
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            if (grid[y][x] == 0) {
                return false;
            }
        }
    }
    
    // Check for possible merges
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            int value = grid[y][x];
            
            // Check right neighbor
            if (x < gridSize - 1 && grid[y][x+1] == value) {
                return false;
            }
            
            // Check bottom neighbor
            if (y < gridSize - 1 && grid[y+1][x] == value) {
                return false;
            }
        }
    }
    
    return true;
}

void Game::update() {
    std::ostringstream ss;
    ss << "Score: " << score;
    scoreText.setString(ss.str());
}

// void Game::render() {
//     window.clear(sf::Color(187, 173, 160));
    
//     switch (currentState) {
//         case GameState::MAIN_MENU:
//             renderMainMenu();
//             break;
//         case GameState::VERSION_MENU:
//             renderVersionMenu();
//             break;
//         case GameState::GAME:
//             renderGame();
//             break;
//     }
    
//     window.display();
// }

void Game::render() {
    window.clear(sf::Color(187, 173, 160));
    
    // 先渲染当前界面
    switch (currentState) {
        case GameState::MAIN_MENU:
            renderMainMenu();
            break;
        case GameState::VERSION_MENU:
            renderVersionMenu();
            break;
        case GameState::GAME:
            renderGame();
            break;
        case GameState::EXIT_CONFIRM:
            // 退出确认时仍然显示之前的界面作为背景
            if (!grid.empty()) {
                renderGame();
            } else if (currentState == GameState::VERSION_MENU) {
                renderVersionMenu();
            } else {
                renderMainMenu();
            }
            break;
    }
    
    // 如果是退出确认状态，再渲染确认对话框
    if (currentState == GameState::EXIT_CONFIRM) {
        window.draw(exitConfirmBackground);
        window.draw(exitConfirmBox);
        window.draw(exitConfirmText);
        window.draw(exitConfirmYesButton);
        window.draw(exitConfirmNoButton);
        window.draw(exitConfirmYesText);
        window.draw(exitConfirmNoText);
    }
    
    window.display();
}

void Game::renderMainMenu() {
    window.draw(titleText);
    for (const auto& button : sizeButtons) {
        window.draw(button);
    }
    for (const auto& text : sizeButtonTexts) {
        window.draw(text);
    }
}

void Game::renderVersionMenu() {
    window.draw(versionTitleText);
    for (const auto& button : versionButtons) {
        window.draw(button);
    }
    for (const auto& text : versionButtonTexts) {
        window.draw(text);
    }
}

void Game::renderGame() {
    // Draw score
    window.draw(scoreText);
    
    // Draw grid background
    sf::RectangleShape gridBackground(sf::Vector2f(
        gridSize * (TILE_SIZE + TILE_MARGIN) + TILE_MARGIN,
        gridSize * (TILE_SIZE + TILE_MARGIN) + TILE_MARGIN
    ));
    gridBackground.setPosition(GRID_OFFSET_X, GRID_OFFSET_Y);
    gridBackground.setFillColor(sf::Color(143, 122, 102));
    window.draw(gridBackground);
    
    // Draw tiles
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            if (grid[y][x] != 0) {
                sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(
                    GRID_OFFSET_X + TILE_MARGIN + x * (TILE_SIZE + TILE_MARGIN),
                    GRID_OFFSET_Y + TILE_MARGIN + y * (TILE_SIZE + TILE_MARGIN)
                );
                tile.setFillColor(getTileColor(grid[y][x]));
                window.draw(tile);
                
                // Draw tile value
                sf::Text valueText;
                valueText.setFont(font);
                valueText.setString(std::to_string(grid[y][x]));
                valueText.setCharacterSize(32);
                valueText.setFillColor(grid[y][x] < 8 ? sf::Color(119, 110, 101) : sf::Color(249, 246, 242));
                
                sf::FloatRect textRect = valueText.getLocalBounds();
                valueText.setOrigin(textRect.left + textRect.width/2.0f,
                                  textRect.top + textRect.height/2.0f);
                valueText.setPosition(
                    GRID_OFFSET_X + TILE_MARGIN + x * (TILE_SIZE + TILE_MARGIN) + TILE_SIZE/2,
                    GRID_OFFSET_Y + TILE_MARGIN + y * (TILE_SIZE + TILE_MARGIN) + TILE_SIZE/2
                );
                window.draw(valueText);
            }
        }
    }
    
    // Draw game over/won message
    if (gameOver) {
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        
        if (gameWon) {
            gameOverText.setString("You Win!");
            gameOverText.setFillColor(sf::Color(237, 194, 46));
        } else {
            gameOverText.setString("Game Over!");
            gameOverText.setFillColor(sf::Color(119, 110, 101));
        }
        
        sf::FloatRect textRect = gameOverText.getLocalBounds();
        gameOverText.setOrigin(textRect.left + textRect.width/2.0f,
                             textRect.top + textRect.height/2.0f);
        gameOverText.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 30);
        
        window.draw(gameOverText);
        window.draw(restartText);
    }
}

sf::Color Game::getTileColor(int value) const {
    switch (value) {
        case 2:    return tileColors[0];
        case 4:    return tileColors[1];
        case 8:    return tileColors[2];
        case 16:   return tileColors[3];
        case 32:   return tileColors[4];
        case 64:   return tileColors[5];
        case 128:  return tileColors[6];
        case 256:  return tileColors[7];
        case 512:  return tileColors[8];
        case 1024: return tileColors[9];
        case 2048: return tileColors[10];
        default:   return tileColors[11]; // For values > 2048
    }
}