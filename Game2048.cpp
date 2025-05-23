#include "Game2048.h"
#include <stdexcept>
#include <random>
#include <sstream>

// 窗口的宽度
constexpr int WINDOW_WIDTH = 800;
// 窗口的高度
constexpr int WINDOW_HEIGHT = 800;
// 网格在 x 轴方向的偏移量
int GRID_OFFSET_X = 100;
// 网格在 y 轴方向的偏移量
int GRID_OFFSET_Y = 200;
// 每个数字方块的大小
int TILE_SIZE = 30;
// 数字方块之间的间距
int TILE_MARGIN = 5;
// 网格线的粗细
constexpr int GRID_LINE_THICKNESS = 4; // 增加网格线粗细，让网格更直观
// 网格线的颜色
const sf::Color GRID_LINE_COLOR = sf::Color(119, 110, 101);

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2048 Game"),
               currentState(GameState::MAIN_MENU),
               currentVersion(GameVersion::ORIGINAL),
               gridSize(4),
               score(0),
               gameOver(false),
               gameWon(false),
               animationProgress(0.0f),
               animationDuration(1.0f) { // 将动画持续时间从 0.5 秒增加到 1.0 秒
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
    exitConfirmYesText.setPosition(WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30);
    
    exitConfirmNoText.setFont(font);
    exitConfirmNoText.setString("No");
    exitConfirmNoText.setCharacterSize(24);
    exitConfirmNoText.setFillColor(sf::Color::White);
    exitConfirmNoText.setPosition(WINDOW_WIDTH/2 + 65, WINDOW_HEIGHT/2 + 30);
}

void Game::initializeUI() {
    setupMainMenu();
    setupVersionMenu();
    
    // 游戏分数显示文本
    scoreText.setFont(font);
    scoreText.setCharacterSize(32);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, 20);
    
    // 游戏结束显示文本
    gameOverText.setFont(font);
    gameOverText.setString("Game Over!");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 - 50);
    
    // 重新开始提示文本
    restartText.setFont(font);
    restartText.setString("Press R to Restart");
    restartText.setCharacterSize(24);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 20);
}

void Game::setupMainMenu() {
    // 主菜单标题文本
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
        // 主菜单选择网格大小的按钮
        sizeButtons[i].setSize(sf::Vector2f(200, 80));
        sizeButtons[i].setPosition(300, 250 + i * 120);
        sizeButtons[i].setFillColor(buttonColors[i]);
        
        // 主菜单选择网格大小按钮上的文本
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
    // 版本选择菜单标题文本
    versionTitleText.setFont(font);
    versionTitleText.setString("Choose Version");
    versionTitleText.setCharacterSize(48);
    versionTitleText.setFillColor(sf::Color::White);
    versionTitleText.setPosition(280, 100);
    
    const std::array<std::string, 2> versionLabels = {
        "Original Version (Arrow Keys)",
        "Diagonal Only (Q/E/Z/C Keys)"
    };
    
    for (size_t i = 0; i < versionButtons.size(); ++i) {
        // 版本选择菜单的按钮
        versionButtons[i].setSize(sf::Vector2f(400, 80));
        versionButtons[i].setPosition(200, 250 + i * 120);
        versionButtons[i].setFillColor(sf::Color(143, 122, 102));
        
        // 版本选择菜单按钮上的文本
        versionButtonTexts[i].setFont(font);
        versionButtonTexts[i].setString(versionLabels[i]);
        versionButtonTexts[i].setCharacterSize(28);
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
        // 关闭窗口事件
        if (event.type == sf::Event::Closed) {
            currentState = GameState::EXIT_CONFIRM;
        }

        // 键盘输入事件
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                currentState = GameState::EXIT_CONFIRM;
            }

            // 退出确认界面处理
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
    
    if (currentVersion == GameVersion::ORIGINAL) {
        // 原始版本：只支持上下左右
        switch (key) {
            case sf::Keyboard::Up:    moved = moveTiles(0, -1); break;
            case sf::Keyboard::Down:  moved = moveTiles(0, 1);  break;
            case sf::Keyboard::Left:  moved = moveTiles(-1, 0); break;
            case sf::Keyboard::Right: moved = moveTiles(1, 0);  break;
            default: break;
        }
    } else {
        // 修改版本：仅支持斜向移动
        switch (key) {
            case sf::Keyboard::Q:     moved = moveTiles(-1, -1); break; // 左上
            case sf::Keyboard::E:     moved = moveTiles(1, -1); break;  // 右上
            case sf::Keyboard::Z:     moved = moveTiles(-1, 1); break;  // 左下
            case sf::Keyboard::C:     moved = moveTiles(1, 1); break;   // 右下
            default: break;
        }
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
    
    // 计算新的网格布局
    calculateGridLayout();
    
    // 添加初始方块
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
        
        // 添加新方块动画
        newTileAnimations.push_back({
            getTilePosition(x, y),
            0.0f // 初始进度为0
        });
    }
}

bool Game::moveTiles(int dx, int dy) {
    bool moved = false;
    std::vector<std::vector<bool>> merged(gridSize, std::vector<bool>(gridSize, false));
    tileAnimations.clear(); // 清空之前的动画记录

    // 根据移动方向确定遍历顺序
    const bool horizontal = (dx != 0);
    const int start = (dx > 0 || dy > 0) ? gridSize - 1 : 0;
    const int step = (dx > 0 || dy > 0) ? -1 : 1;

    // 遍历每个可能的行或列
    for (int i = 0; i < gridSize; ++i) {
        for (int j = start; j >= 0 && j < gridSize; j += step) {
            // 获取当前格子的坐标
            int x = horizontal ? j : i;
            int y = horizontal ? i : j;
            
            if (grid[y][x] == 0) continue;

            // 计算移动方向和目标位置
            int newX = x;
            int newY = y;
            int prevX = x;
            int prevY = y;
            bool hasMerged = false;

            while (true) {
                int nextX = newX + dx;
                int nextY = newY + dy;

                // 边界检查
                if (nextX < 0 || nextX >= gridSize || nextY < 0 || nextY >= gridSize) break;

                // 如果目标位置为空，继续移动
                if (grid[nextY][nextX] == 0) {
                    prevX = newX;
                    prevY = newY;
                    newX = nextX;
                    newY = nextY;
                    moved = true;
                } 
                // 如果目标位置有相同值且未合并过
                else if (grid[nextY][nextX] == grid[y][x] && !merged[nextY][nextX]) {
                    merged[nextY][nextX] = true;
                    grid[nextY][nextX] *= 2;
                    score += grid[nextY][nextX];
                    grid[y][x] = 0;
                    moved = true;
                    hasMerged = true;
                    break;
                } 
                // 其他情况停止移动
                else {
                    break;
                }
            }

            // 如果没有合并但移动了位置
            if (!hasMerged && (newX != x || newY != y)) {
                grid[newY][newX] = grid[y][x];
                grid[y][x] = 0;
                tileAnimations.emplace_back(getTilePosition(x, y), getTilePosition(newX, newY));
                moved = true;
            }

            if (moved) {
                tileAnimations.emplace_back(getTilePosition(x, y), getTilePosition(newX, newY));
            }
        }
    }

    if (moved) {
        animationProgress = 0.0f; // 重置动画进度
    }

    return moved;
}

// TODO: 实现连续合并版本的2048

bool Game::moveTilesContinuous(int dx, int dy) {
    bool moved = false;
    
    // 创建一个标记矩阵来跟踪哪些格子已经合并过
    std::vector<std::vector<bool>> merged(gridSize, std::vector<bool>(gridSize, false));
    
    // 确定遍历顺序
    int startX = (dx >= 0) ? 0 : gridSize - 1;
    int endX = (dx >= 0) ? gridSize : -1;
    int stepX = (dx >= 0) ? 1 : -1;
    
    int startY = (dy >= 0) ? 0 : gridSize - 1;
    int endY = (dy >= 0) ? gridSize : -1;
    int stepY = (dy >= 0) ? 1 : -1;
    
    for (int y = startY; y != endY; y += stepY) {
        for (int x = startX; x != endX; x += stepX) {
            if (grid[y][x] == 0) continue;
            
            int newX = x;
            int newY = y;
            int currentX = x;
            int currentY = y;
            bool canMerge = false;
            
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
                } else if (grid[currentY][currentX] == grid[y][x] && !merged[currentY][currentX]) {
                    newX = currentX;
                    newY = currentY;
                    canMerge = true;
                    break;
                } else {
                    break;
                }
            }
            
            if (newX != x || newY != y) {
                if (canMerge) {
                    // 合并方块
                    grid[newY][newX] *= 2;
                    score += grid[newY][newX];
                    merged[newY][newX] = true; // 标记已合并
                    if (grid[newY][newX] == 2048) {
                        gameWon = true;
                    }
                } else {
                    // 移动方块到空位
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
    if (currentVersion == GameVersion::ORIGINAL) {
        return Game::isGameOver_grid();
    } else {
        return Game::isGameOVer_diagonal();
    }
}

bool Game::isGameOVer_diagonal() const {
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            if (grid[y][x] == 0) {
                return false;
            }
        }
    }
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            int value = grid[y][x];
            
            // 检查右下方邻居
            if (x < gridSize - 1 && y < gridSize - 1 && grid[y+1][x+1] == value) {
                return false;
            }
            
            // 检查左下方邻居
            if (y < gridSize - 1 && x > 0 && grid[y+1][x-1] == value) {
                return false;
            }
        }
    }
    
    return true;
}

bool Game::isGameOver_grid() const {
    // 检查是否有空位
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            if (grid[y][x] == 0) {
                return false;
            }
        }
    }
    
    // 检查是否有可以合并的方块
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            int value = grid[y][x];
            
            // 检查右侧邻居
            if (x < gridSize - 1 && grid[y][x+1] == value) {
                return false;
            }
            
            // 检查下方邻居
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

    if (!tileAnimations.empty()) {
        animationProgress += (1.0f / (60.0f * animationDuration)); 
        if (animationProgress >= 1.0f) {
            animationProgress = 1.0f;
            tileAnimations.clear();
        }
    }

    // 更新新方块生成动画
    for (auto& anim : newTileAnimations) {
        anim.progress += (1.0f / (60.0f * spawnAnimationDuration));
        if (anim.progress > 1.0f) anim.progress = 1.0f;
    }
    // 移除已完成的动画
    newTileAnimations.erase(
        std::remove_if(newTileAnimations.begin(), newTileAnimations.end(),
            [](const NewTileAnimation& a) { return a.progress >= 1.0f; }),
        newTileAnimations.end()
    );
}

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
    // 绘制分数
    window.draw(scoreText);
    
    // 绘制网格背景
    if (currentVersion == GameVersion::ORIGINAL) {
        // 原始版本 - 绘制整体背景和网格线
        sf::RectangleShape gridBackground(sf::Vector2f(
            gridSize * (TILE_SIZE + TILE_MARGIN) + TILE_MARGIN,
            gridSize * (TILE_SIZE + TILE_MARGIN) + TILE_MARGIN
        ));
        gridBackground.setPosition(GRID_OFFSET_X, GRID_OFFSET_Y);
        gridBackground.setFillColor(sf::Color(143, 122, 102));
        window.draw(gridBackground);
    } else {
        // 修改版本 - 绘制黑白棋盘背景
        for (int y = 0; y < gridSize; ++y) {
            for (int x = 0; x < gridSize; ++x) {
                sf::RectangleShape cellBackground(sf::Vector2f(
                    TILE_SIZE + TILE_MARGIN, 
                    TILE_SIZE + TILE_MARGIN
                ));
                cellBackground.setPosition(
                    GRID_OFFSET_X + x * (TILE_SIZE + TILE_MARGIN),
                    GRID_OFFSET_Y + y * (TILE_SIZE + TILE_MARGIN)
                );
                cellBackground.setFillColor(getCellBackgroundColor(x, y));
                window.draw(cellBackground);
            }
        }
    }
    
    // 绘制数字格子
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            if (grid[y][x] != 0) {
                sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                
                // 调整位置 - 修改版本需要居中
                sf::Vector2f position;
                if (currentVersion == GameVersion::MODIFIED) {
                    position = sf::Vector2f(
                        GRID_OFFSET_X + x * (TILE_SIZE + TILE_MARGIN) + TILE_MARGIN/2,
                        GRID_OFFSET_Y + y * (TILE_SIZE + TILE_MARGIN) + TILE_MARGIN/2
                    );
                } else {
                    position = sf::Vector2f(
                        GRID_OFFSET_X + TILE_MARGIN + x * (TILE_SIZE + TILE_MARGIN),
                        GRID_OFFSET_Y + TILE_MARGIN + y * (TILE_SIZE + TILE_MARGIN)
                    );
                }

                float scale = 1.0f;
                for (const auto& anim : newTileAnimations) {
                    if (position == anim.position) {
                        // 使用缓动函数实现更平滑的动画
                        scale = 0.2f + 0.8f * std::sin(anim.progress * 3.14159f/2);
                        break;
                    }
                }
                tile.setScale(scale, scale);

                // 如果有动画，根据动画进度更新位置
                for (size_t i = 0; i < tileAnimations.size(); ++i) {
                    if (getTilePosition(x, y) == tileAnimations[i].first) {
                        position.x = tileAnimations[i].first.x + (tileAnimations[i].second.x - tileAnimations[i].first.x) * animationProgress;
                        position.y = tileAnimations[i].first.y + (tileAnimations[i].second.y - tileAnimations[i].first.y) * animationProgress;
                        break;
                    }
                }

                // 调整位置中心点以保持居中缩放
                tile.setOrigin(TILE_SIZE/2 * (1 - scale), TILE_SIZE/2 * (1 - scale)); 

                tile.setPosition(position);
                tile.setFillColor(getTileColor(grid[y][x]));
                window.draw(tile);
                
                // 绘制数字
                sf::Text valueText;
                valueText.setFont(font);
                valueText.setString(std::to_string(grid[y][x]));
                valueText.setCharacterSize(32);
                valueText.setFillColor(grid[y][x] < 8 ? sf::Color(119, 110, 101) : sf::Color(249, 246, 242));
                
                sf::FloatRect textRect = valueText.getLocalBounds();
                valueText.setOrigin(textRect.left + textRect.width/2.0f,
                                  textRect.top + textRect.height/2.0f);
                
                // 调整数字位置 - 修改版本需要居中
                if (currentVersion == GameVersion::MODIFIED) {
                    valueText.setPosition(
                        position.x + TILE_SIZE/2,
                        position.y + TILE_SIZE/2
                    );
                } else {
                    valueText.setPosition(
                        position.x + TILE_SIZE/2,
                        position.y + TILE_SIZE/2
                    );
                }
                
                window.draw(valueText);
            }
        }
    }
    
    // 绘制游戏结束/胜利消息（保持不变）
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

// 在构造函数之后添加这些函数实现
void Game::calculateGridLayout() {
    // 根据网格大小计算方块尺寸和间距
    const float maxGridWidth = WINDOW_WIDTH * 0.8f;  // 网格最大宽度为窗口宽度的80%
    const float maxGridHeight = WINDOW_HEIGHT * 0.6f; // 网格最大高度为窗口高度的60%
    
    // 计算适合的方块大小
    float tileSizeWithMargin = std::min(
        maxGridWidth / (gridSize + 0.5f),  // 加0.5f为边距留空间
        maxGridHeight / (gridSize + 0.5f)
    );
    
    // 设置方块大小和间距（间距为大小的1/5）
    TILE_SIZE = static_cast<int>(tileSizeWithMargin * 0.83f); // 方块占83%
    TILE_MARGIN = static_cast<int>(tileSizeWithMargin * 0.17f); // 间距占17%
    
    // 计算网格起始位置（居中偏下）
    GRID_OFFSET_X = (WINDOW_WIDTH - (gridSize * (TILE_SIZE + TILE_MARGIN) + TILE_MARGIN)) / 2;
    GRID_OFFSET_Y = WINDOW_HEIGHT * 0.6f - (gridSize * (TILE_SIZE + TILE_MARGIN) + TILE_MARGIN) / 2;
}

sf::Vector2f Game::getTilePosition(int x, int y) const {
    return sf::Vector2f(
        GRID_OFFSET_X + x * (TILE_SIZE + TILE_MARGIN),
        GRID_OFFSET_Y + y * (TILE_SIZE + TILE_MARGIN)
    );
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

sf::Color Game::getCellBackgroundColor(int x, int y) const {
    if (currentVersion == GameVersion::MODIFIED) {
        // 修改版本使用更柔和的棋盘样式
        return (x + y) % 2 == 0 ? sf::Color(220, 220, 220) : sf::Color(170, 170, 170);
    }
    return sf::Color::Transparent; // 原始版本不需要这个功能
}