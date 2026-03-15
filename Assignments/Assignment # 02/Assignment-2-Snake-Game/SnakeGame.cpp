#include "SnakeGame.h"
#include <math.h>

using namespace std;
using namespace sf;

// ═══════════════════════════════════════════════
//  CONSTRUCTOR / DESTRUCTOR
// ═══════════════════════════════════════════════

SnakeGame::SnakeGame()
    : window(VideoMode(Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Snake"),
    leaderboard("snake_leaderboard.txt")
{
    srand(static_cast<unsigned>(time(0)));
    window.setFramerateLimit(60);

    snakeLogoSprite = nullptr;
    appleSprite = nullptr;
    eatBuffer = nullptr;
    gameOverBuffer = nullptr;
    clickBuffer = nullptr;
    eatSound = nullptr;
    gameOverSound = nullptr;
    clickSound = nullptr;

    fontLoaded = false;
    logoLoaded = false;
    appleTextureLoaded = false;
    eatLoaded = false;
    gameOverLoaded = false;
    clickLoaded = false;

    initColors();
    loadAssets();

    state = GameState::STATE_MENU;
    menuSelection = 0;
    gameOverSelection = 0;
    foodPulseTimer = 0.f;
    menuHoverTimer = 0.f;
    tickTimer = 0.f;
    tickDelay = 0.15f;
    minTickDelay = 0.06f;
    countdownValue = 3;
    activeNameField = 0;
    snake1Dead = false;
    snake2Dead = false;
    score1 = 0;
    score2 = 0;
    dir1 = Direction::DIR_RIGHT;
    dir2 = Direction::DIR_LEFT;
    lastDir1 = Direction::DIR_RIGHT;
    lastDir2 = Direction::DIR_LEFT;
    snake1Length = 0;
    snake2Length = 0;
    particleCount = 0;
    inputName1[0] = '\0';
    inputName2[0] = '\0';
    name1[0] = '\0';
    name2[0] = '\0';
    resultMessage[0] = '\0';
}

SnakeGame::~SnakeGame() {
    if (bgm.getStatus() == Music::Status::Playing) {
        bgm.stop();
    }
    if (snakeLogoSprite) delete snakeLogoSprite;
    if (appleSprite) delete appleSprite;
    if (eatSound) delete eatSound;
    if (gameOverSound) delete gameOverSound;
    if (clickSound) delete clickSound;
    if (eatBuffer) delete eatBuffer;
    if (gameOverBuffer) delete gameOverBuffer;
    if (clickBuffer) delete clickBuffer;
}

// ═══════════════════════════════════════════════
//  INITIALIZATION
// ═══════════════════════════════════════════════

void SnakeGame::initColors() {
    bgColor = Color(18, 18, 24);
    gridLineColor = Color(30, 30, 42);
    snake1HeadColor = Color(0, 230, 118);
    snake1BodyColor = Color(0, 180, 90);
    snake2HeadColor = Color(100, 140, 255);
    snake2BodyColor = Color(70, 110, 220);
    foodColor = Color(255, 60, 80);
    uiPanelColor = Color(25, 25, 36);
    accentColor = Color(180, 90, 255);
    textColor = Color(240, 240, 250);
    dimTextColor = Color(120, 120, 150);
}

void SnakeGame::loadAssets() {
    // Font
    if (font.openFromFile("assets/font.ttf")) {
        fontLoaded = true;
    }

    // Snake logo
    if (snakeLogoTexture.loadFromFile("assets/snake_logo.png")) {
        snakeLogoTexture.setSmooth(true);
        snakeLogoSprite = new Sprite(snakeLogoTexture);
        float scaleX = 150.f / snakeLogoTexture.getSize().x;
        float scaleY = 150.f / snakeLogoTexture.getSize().y;
        snakeLogoSprite->setScale({ scaleX, scaleY });
        logoLoaded = true;
    }

    // Apple texture
    if (appleTexture.loadFromFile("assets/apple.png")) {
        appleTexture.setSmooth(true);
        appleSprite = new Sprite(appleTexture);
        float scaleX = (float)CELL_SIZE / appleTexture.getSize().x;
        float scaleY = (float)CELL_SIZE / appleTexture.getSize().y;
        appleSprite->setScale({ scaleX, scaleY });
        appleTextureLoaded = true;
    }

    // Audio
    if (bgm.openFromFile("assets/bgm.ogg")) {
        bgm.setLooping(true);
        bgm.setVolume(30.f);
    }

    eatBuffer = new SoundBuffer;
    if (eatBuffer->loadFromFile("assets/eat.wav")) {
        eatSound = new Sound(*eatBuffer);
        eatSound->setVolume(60.f);
        eatLoaded = true;
    }

    gameOverBuffer = new SoundBuffer;
    if (gameOverBuffer->loadFromFile("assets/gameover.wav")) {
        gameOverSound = new Sound(*gameOverBuffer);
        gameOverSound->setVolume(70.f);
        gameOverLoaded = true;
    }

    clickBuffer = new SoundBuffer;
    if (clickBuffer->loadFromFile("assets/click.wav")) {
        clickSound = new Sound(*clickBuffer);
        clickSound->setVolume(50.f);
        clickLoaded = true;
    }
}

// ═══════════════════════════════════════════════
//  GAME RESET
// ═══════════════════════════════════════════════

void SnakeGame::resetGame() {
    // Snake 1 starts left side
    snake1[0] = Vector2i(4, GRID_ROWS / 2);
    snake1Length = 1;
    dir1 = Direction::DIR_RIGHT;
    lastDir1 = Direction::DIR_RIGHT;
    score1 = 0;

    // Snake 2 starts right side
    snake2[0] = Vector2i(GRID_COLS - 5, GRID_ROWS / 2);
    snake2Length = 1;
    dir2 = Direction::DIR_LEFT;
    lastDir2 = Direction::DIR_LEFT;
    score2 = 0;

    snake1Dead = false;
    snake2Dead = false;
    tickDelay = 0.15f;
    tickTimer = 0.f;
    particleCount = 0;
    foodPulseTimer = 0.f;

    spawnFood();
}

void SnakeGame::spawnFood() {
    bool valid = false;
    while (!valid) {
        foodPos.x = 1 + rand() % (GRID_COLS - 2);
        foodPos.y = 1 + rand() % (GRID_ROWS - 2);
        valid = true;

        for (int i = 0; i < snake1Length; i++) {
            if (snake1[i] == foodPos) { valid = false; break; }
        }
        if (valid) {
            for (int i = 0; i < snake2Length; i++) {
                if (snake2[i] == foodPos) { valid = false; break; }
            }
        }
    }
}

// ═══════════════════════════════════════════════
//  MAIN GAME LOOP
// ═══════════════════════════════════════════════

void SnakeGame::run() {
    while (window.isOpen() && state != GameState::STATE_EXIT) {
        float dt = gameClock.restart().asSeconds();
        handleEvents();
        update(dt);
        render();
    }
    window.close();
}

// ═══════════════════════════════════════════════
//  EVENT HANDLING (SFML 3.0 std::optional)
// ═══════════════════════════════════════════════

void SnakeGame::handleEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            state = GameState::STATE_EXIT;
            return;
        }

        switch (state) {
        case GameState::STATE_MENU:        handleMenuEvent(*event);        break;
        case GameState::STATE_NAME_INPUT:  handleNameInputEvent(*event);   break;
        case GameState::STATE_PLAYING:     handlePlayingEvent(*event);     break;
        case GameState::STATE_PAUSED:      handlePausedEvent(*event);      break;
        case GameState::STATE_GAME_OVER:   handleGameOverEvent(*event);    break;
        case GameState::STATE_LEADERBOARD: handleLeaderboardEvent(*event); break;
        default: break;
        }
    }
}

void SnakeGame::handleMenuEvent(const Event& e) {
    if (const auto* key = e.getIf<Event::KeyPressed>()) {
        if (key->code == Keyboard::Key::Up || key->code == Keyboard::Key::W) {
            menuSelection = (menuSelection - 1 + 3) % 3;
            if (clickLoaded) clickSound->play();
        }
        if (key->code == Keyboard::Key::Down || key->code == Keyboard::Key::S) {
            menuSelection = (menuSelection + 1) % 3;
            if (clickLoaded) clickSound->play();
        }
        if (key->code == Keyboard::Key::Enter) {
            if (clickLoaded) clickSound->play();
            if (menuSelection == 0) {
                inputName1[0] = '\0';
                inputName2[0] = '\0';
                activeNameField = 0;
                state = GameState::STATE_NAME_INPUT;
            }
            else if (menuSelection == 1) {
                state = GameState::STATE_LEADERBOARD;
            }
            else if (menuSelection == 2) {
                state = GameState::STATE_EXIT;
            }
        }
    }
    if (const auto* mb = e.getIf<Event::MouseButtonPressed>()) {
        if (mb->button == Mouse::Button::Left) {
            Vector2i mousePos = Mouse::getPosition(window);
            float btnX = WINDOW_WIDTH / 2.f - 120.f;
            float btnY_start = 380.f;
            float btnW = 240.f;
            float btnH = 50.f;
            float gap = 15.f;

            for (int i = 0; i < 3; i++) {
                FloatRect btnRect({ btnX, btnY_start + i * (btnH + gap) }, { btnW, btnH });
                if (btnRect.contains(Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) {
                    menuSelection = i;
                    if (clickLoaded) clickSound->play();
                    if (i == 0) {
                        inputName1[0] = '\0';
                        inputName2[0] = '\0';
                        activeNameField = 0;
                        state = GameState::STATE_NAME_INPUT;
                    }
                    else if (i == 1) {
                        state = GameState::STATE_LEADERBOARD;
                    }
                    else if (i == 2) {
                        state = GameState::STATE_EXIT;
                    }
                }
            }
        }
    }
}

void SnakeGame::handleNameInputEvent(const Event& e) {
    if (const auto* textEvent = e.getIf<Event::TextEntered>()) {
        char32_t unicode = textEvent->unicode;
        char* currentName = (activeNameField == 0) ? inputName1 : inputName2;
        int currentLen = textLength(currentName);

        if (unicode == '\b') {
            if (currentLen > 0) {
                currentName[currentLen - 1] = '\0';
            }
        }
        else if (unicode == '\r' || unicode == '\n') {
            if (currentLen > 0) {
                if (activeNameField == 0) {
                    activeNameField = 1;
                }
                else {
                    textCopy(name1, inputName1, 50);
                    textCopy(name2, inputName2, 50);
                    resetGame();
                    countdownValue = 3;
                    countdownClock.restart();
                    state = GameState::STATE_COUNTDOWN;
                    if (bgm.getStatus() != Music::Status::Playing) {
                        bgm.play();
                    }
                }
            }
        }
        else if (unicode == '\t') {
            if (activeNameField == 0 && textLength(inputName1) > 0) {
                activeNameField = 1;
            }
        }
        else if (unicode >= 32 && unicode < 127 && currentLen < 12) {
            if (unicode != ' ') {
                currentName[currentLen] = static_cast<char>(unicode);
                currentName[currentLen + 1] = '\0';
            }
        }
    }
    if (const auto* key = e.getIf<Event::KeyPressed>()) {
        if (key->code == Keyboard::Key::Escape) {
            state = GameState::STATE_MENU;
        }
    }
}

void SnakeGame::handlePlayingEvent(const Event& e) {
    if (const auto* key = e.getIf<Event::KeyPressed>()) {
        // Snake 1 — WASD (validate against last applied direction to prevent reversal)
        if (key->code == Keyboard::Key::W && lastDir1 != Direction::DIR_DOWN)  dir1 = Direction::DIR_UP;
        if (key->code == Keyboard::Key::S && lastDir1 != Direction::DIR_UP)    dir1 = Direction::DIR_DOWN;
        if (key->code == Keyboard::Key::A && lastDir1 != Direction::DIR_RIGHT) dir1 = Direction::DIR_LEFT;
        if (key->code == Keyboard::Key::D && lastDir1 != Direction::DIR_LEFT)  dir1 = Direction::DIR_RIGHT;

        // Snake 2 — Arrow keys (validate against last applied direction to prevent reversal)
        if (key->code == Keyboard::Key::Up && lastDir2 != Direction::DIR_DOWN)  dir2 = Direction::DIR_UP;
        if (key->code == Keyboard::Key::Down && lastDir2 != Direction::DIR_UP)    dir2 = Direction::DIR_DOWN;
        if (key->code == Keyboard::Key::Left && lastDir2 != Direction::DIR_RIGHT) dir2 = Direction::DIR_LEFT;
        if (key->code == Keyboard::Key::Right && lastDir2 != Direction::DIR_LEFT)  dir2 = Direction::DIR_RIGHT;

        // Pause
        if (key->code == Keyboard::Key::Escape || key->code == Keyboard::Key::P) {
            state = GameState::STATE_PAUSED;
        }
    }
}

void SnakeGame::handlePausedEvent(const Event& e) {
    if (const auto* key = e.getIf<Event::KeyPressed>()) {
        if (key->code == Keyboard::Key::Escape || key->code == Keyboard::Key::P) {
            state = GameState::STATE_PLAYING;
        }
        if (key->code == Keyboard::Key::Q) {
            state = GameState::STATE_MENU;
            bgm.stop();
        }
    }
}

void SnakeGame::handleGameOverEvent(const Event& e) {
    if (const auto* key = e.getIf<Event::KeyPressed>()) {
        if (key->code == Keyboard::Key::Up || key->code == Keyboard::Key::W) {
            gameOverSelection = (gameOverSelection - 1 + 3) % 3;
            if (clickLoaded) clickSound->play();
        }
        if (key->code == Keyboard::Key::Down || key->code == Keyboard::Key::S) {
            gameOverSelection = (gameOverSelection + 1) % 3;
            if (clickLoaded) clickSound->play();
        }
        if (key->code == Keyboard::Key::Enter) {
            if (clickLoaded) clickSound->play();
            if (gameOverSelection == 0) {
                resetGame();
                countdownValue = 3;
                countdownClock.restart();
                state = GameState::STATE_COUNTDOWN;
            }
            else if (gameOverSelection == 1) {
                bgm.stop();
                state = GameState::STATE_MENU;
            }
            else if (gameOverSelection == 2) {
                state = GameState::STATE_EXIT;
            }
        }
    }
    if (const auto* mb = e.getIf<Event::MouseButtonPressed>()) {
        if (mb->button == Mouse::Button::Left) {
            Vector2i mousePos = Mouse::getPosition(window);
            float btnX = WINDOW_WIDTH / 2.f - 120.f;
            float btnY_start = WINDOW_HEIGHT / 2.f + 40.f;
            float btnW = 240.f;
            float btnH = 45.f;
            float gap = 12.f;

            for (int i = 0; i < 3; i++) {
                FloatRect btnRect({ btnX, btnY_start + i * (btnH + gap) }, { btnW, btnH });
                if (btnRect.contains(Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) {
                    gameOverSelection = i;
                    if (clickLoaded) clickSound->play();
                    if (i == 0) {
                        resetGame();
                        countdownValue = 3;
                        countdownClock.restart();
                        state = GameState::STATE_COUNTDOWN;
                    }
                    else if (i == 1) {
                        bgm.stop();
                        state = GameState::STATE_MENU;
                    }
                    else if (i == 2) {
                        state = GameState::STATE_EXIT;
                    }
                }
            }
        }
    }
}

void SnakeGame::handleLeaderboardEvent(const Event& e) {
    if (const auto* key = e.getIf<Event::KeyPressed>()) {
        if (key->code == Keyboard::Key::Escape || key->code == Keyboard::Key::Enter) {
            if (clickLoaded) clickSound->play();
            state = GameState::STATE_MENU;
        }
    }
    if (e.getIf<Event::MouseButtonPressed>()) {
        if (clickLoaded) clickSound->play();
        state = GameState::STATE_MENU;
    }
}

// ═══════════════════════════════════════════════
//  UPDATE
// ═══════════════════════════════════════════════

void SnakeGame::update(float dt) {
    menuHoverTimer += dt;
    foodPulseTimer += dt;
    updateParticles(dt);

    switch (state) {
    case GameState::STATE_COUNTDOWN: updateCountdown(dt); break;
    case GameState::STATE_PLAYING:   updatePlaying(dt);   break;
    default: break;
    }
}

void SnakeGame::updateCountdown(float dt) {
    float elapsed = countdownClock.getElapsedTime().asSeconds();
    if (elapsed >= 1.f) {
        countdownValue--;
        countdownClock.restart();
        if (countdownValue <= 0) {
            state = GameState::STATE_PLAYING;
            gameClock.restart();
            tickTimer = 0.f;
        }
    }
}

void SnakeGame::updatePlaying(float dt) {
    tickTimer += dt;

    if (tickTimer >= tickDelay) {
        tickTimer = 0.f;

        // Lock in directions for this tick
        lastDir1 = dir1;
        lastDir2 = dir2;

        // Save old tail positions before moving (used for correct growth)
        Vector2i oldTail1 = snake1[snake1Length - 1];
        Vector2i oldTail2 = snake2[snake2Length - 1];

        // Move both snakes
        moveSnake(snake1, snake1Length, dir1);
        moveSnake(snake2, snake2Length, dir2);

        // Check collisions before eating/growth
        checkCollisions();

        // Snake 1 eats food (only if alive)
        if (!snake1Dead && snake1[0] == foodPos) {
            score1++;
            if (snake1Length < MAX_SNAKE_LENGTH) {
                snake1[snake1Length] = oldTail1;
                snake1Length++;
            }
            if (eatLoaded) eatSound->play();
            spawnParticles(
                Vector2f(static_cast<float>(foodPos.x * CELL_SIZE + CELL_SIZE / 2),
                    static_cast<float>(foodPos.y * CELL_SIZE + TOP_BAR_HEIGHT + CELL_SIZE / 2)),
                foodColor, 12);
            spawnFood();
            if (tickDelay > minTickDelay) tickDelay -= 0.002f;
        }

        // Snake 2 eats food (only if alive)
        if (!snake2Dead && snake2[0] == foodPos) {
            score2++;
            if (snake2Length < MAX_SNAKE_LENGTH) {
                snake2[snake2Length] = oldTail2;
                snake2Length++;
            }
            if (eatLoaded) eatSound->play();
            spawnParticles(
                Vector2f(static_cast<float>(foodPos.x * CELL_SIZE + CELL_SIZE / 2),
                    static_cast<float>(foodPos.y * CELL_SIZE + TOP_BAR_HEIGHT + CELL_SIZE / 2)),
                foodColor, 12);
            spawnFood();
            if (tickDelay > minTickDelay) tickDelay -= 0.002f;
        }

        if (snake1Dead || snake2Dead) {
            determineResult();
            if (gameOverLoaded) gameOverSound->play();
            leaderboard.addRecord(name1, "Snake", score1);
            leaderboard.addRecord(name2, "Snake", score2);
            gameOverSelection = 0;
            state = GameState::STATE_GAME_OVER;
        }
    }
}

void SnakeGame::updateParticles(float dt) {
    int i = 0;
    while (i < particleCount) {
        particles[i].pos.x += particles[i].vel.x * dt;
        particles[i].pos.y += particles[i].vel.y * dt;
        particles[i].life -= dt;
        if (particles[i].life <= 0.f) {
            // Remove by swapping with last
            particles[i] = particles[particleCount - 1];
            particleCount--;
        }
        else {
            i++;
        }
    }
}

// ═══════════════════════════════════════════════
//  GAME LOGIC
// ══��════════════════════════════════════════════

void SnakeGame::moveSnake(Vector2i* snake, int length, int dir) {
    for (int i = length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    switch (dir) {
    case Direction::DIR_UP:    snake[0].y -= 1; break;
    case Direction::DIR_DOWN:  snake[0].y += 1; break;
    case Direction::DIR_LEFT:  snake[0].x -= 1; break;
    case Direction::DIR_RIGHT: snake[0].x += 1; break;
    }
}

bool SnakeGame::checkWallCollision(const Vector2i& head) {
    return (head.x < 0 || head.x >= GRID_COLS || head.y < 0 || head.y >= GRID_ROWS);
}

bool SnakeGame::checkSelfCollision(const Vector2i* snake, int length) {
    for (int i = 1; i < length; i++) {
        if (snake[0] == snake[i]) return true;
    }
    return false;
}

bool SnakeGame::checkBodyCollision(const Vector2i& head, const Vector2i* otherSnake, int otherLength) {
    for (int i = 0; i < otherLength; i++) {
        if (head == otherSnake[i]) return true;
    }
    return false;
}

void SnakeGame::checkCollisions() {
    if (checkWallCollision(snake1[0])) snake1Dead = true;
    if (checkWallCollision(snake2[0])) snake2Dead = true;
    if (checkSelfCollision(snake1, snake1Length)) snake1Dead = true;
    if (checkSelfCollision(snake2, snake2Length)) snake2Dead = true;
    if (snake1[0] == snake2[0]) { snake1Dead = true; snake2Dead = true; }
    if (!snake1Dead && checkBodyCollision(snake1[0], snake2, snake2Length)) snake1Dead = true;
    if (!snake2Dead && checkBodyCollision(snake2[0], snake1, snake1Length)) snake2Dead = true;
}

void SnakeGame::determineResult() {
    // Score is the primary factor; survival is only a tiebreaker
    if (score1 > score2) {
        textCopy(resultMessage, name1, 120);
        textAppend(resultMessage, " Wins!", 120);
    }
    else if (score2 > score1) {
        textCopy(resultMessage, name2, 120);
        textAppend(resultMessage, " Wins!", 120);
    }
    else {
        // Scores tied — surviving player wins
        if (snake1Dead && !snake2Dead) {
            textCopy(resultMessage, name2, 120);
            textAppend(resultMessage, " Wins!", 120);
        }
        else if (snake2Dead && !snake1Dead) {
            textCopy(resultMessage, name1, 120);
            textAppend(resultMessage, " Wins!", 120);
        }
        else {
            textCopy(resultMessage, "It's a Draw!", 120);
        }
    }
}

// ═══════════════════════════════════════════════
//  RENDERING
// ═══════════════════════════════════════════════

void SnakeGame::render() {
    window.clear(bgColor);

    switch (state) {
    case GameState::STATE_MENU:        renderMenu();        break;
    case GameState::STATE_NAME_INPUT:  renderNameInput();   break;
    case GameState::STATE_COUNTDOWN:   renderCountdown();   break;
    case GameState::STATE_PLAYING:     renderPlaying();     break;
    case GameState::STATE_PAUSED:      renderPaused();      break;
    case GameState::STATE_GAME_OVER:   renderGameOver();    break;
    case GameState::STATE_LEADERBOARD: renderLeaderboard(); break;
    default: break;
    }

    window.display();
}

void SnakeGame::renderMenu() {
    if (!fontLoaded) return;

    // Title
    Text title(font, "SNAKE", 72);
    title.setFillColor(accentColor);
    title.setStyle(Text::Bold);
    FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin({ titleBounds.size.x / 2.f, titleBounds.size.y / 2.f });
    title.setPosition({ WINDOW_WIDTH / 2.f, 100.f });
    window.draw(title);

    // Subtitle
    Text subtitle(font, "Two Player Battle", 18);
    subtitle.setFillColor(dimTextColor);
    FloatRect subBounds = subtitle.getLocalBounds();
    subtitle.setOrigin({ subBounds.size.x / 2.f, subBounds.size.y / 2.f });
    subtitle.setPosition({ WINDOW_WIDTH / 2.f, 155.f });
    window.draw(subtitle);

    // Logo
    if (logoLoaded && snakeLogoSprite) {
        FloatRect logoBounds = snakeLogoSprite->getGlobalBounds();
        snakeLogoSprite->setPosition({ WINDOW_WIDTH / 2.f - logoBounds.size.x / 2.f, 180.f });
        window.draw(*snakeLogoSprite);
    }

    // Menu Buttons
    const char* options[] = { "Start Game", "Leaderboard", "Exit" };
    float btnX = WINDOW_WIDTH / 2.f - 120.f;
    float btnY = 380.f;
    float btnW = 240.f;
    float btnH = 50.f;
    float gap = 15.f;

    Vector2i mousePos = Mouse::getPosition(window);

    for (int i = 0; i < 3; i++) {
        FloatRect btnRect({ btnX, btnY + i * (btnH + gap) }, { btnW, btnH });
        bool hovered = btnRect.contains(Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));
        bool selected = (menuSelection == i);

        RectangleShape btn({ btnW, btnH });
        btn.setPosition({ btnRect.position.x, btnRect.position.y });

        if (selected || hovered) {
            btn.setFillColor(accentColor);
            btn.setOutlineThickness(0);
            if (hovered) menuSelection = i;
        }
        else {
            btn.setFillColor(Color(35, 35, 50));
            btn.setOutlineColor(Color(60, 60, 80));
            btn.setOutlineThickness(1.f);
        }
        window.draw(btn);

        Text btnText(font, options[i], 20);
        btnText.setFillColor(textColor);
        FloatRect textBounds = btnText.getLocalBounds();
        btnText.setOrigin({ textBounds.size.x / 2.f, textBounds.size.y / 2.f });
        btnText.setPosition({ btnRect.position.x + btnW / 2.f, btnRect.position.y + btnH / 2.f - 3.f });
        window.draw(btnText);
    }

    // Controls hint
    Text hint(font, "Player 1: WASD  |  Player 2: Arrow Keys", 13);
    hint.setFillColor(dimTextColor);
    FloatRect hintBounds = hint.getLocalBounds();
    hint.setOrigin({ hintBounds.size.x / 2.f, 0.f });
    hint.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 50.f });
    window.draw(hint);

    Text hint2(font, "Navigate: W/S or Up/Down  |  Select: Enter", 12);
    hint2.setFillColor(Color(80, 80, 110));
    FloatRect hint2Bounds = hint2.getLocalBounds();
    hint2.setOrigin({ hint2Bounds.size.x / 2.f, 0.f });
    hint2.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 30.f });
    window.draw(hint2);
}

void SnakeGame::renderNameInput() {
    if (!fontLoaded) return;

    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(10, 10, 18, 240));
    window.draw(overlay);

    Text title(font, "Enter Player Names", 32);
    title.setFillColor(accentColor);
    FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin({ titleBounds.size.x / 2.f, titleBounds.size.y / 2.f });
    title.setPosition({ WINDOW_WIDTH / 2.f, 120.f });
    window.draw(title);

    const char* labels[] = { "Player 1 (WASD):", "Player 2 (Arrows):" };
    char* names[] = { inputName1, inputName2 };
    Color playerColors[] = { snake1HeadColor, snake2HeadColor };

    for (int i = 0; i < 2; i++) {
        float yPos = 220.f + i * 130.f;

        Text label(font, labels[i], 18);
        label.setFillColor(playerColors[i]);
        FloatRect lBounds = label.getLocalBounds();
        label.setOrigin({ lBounds.size.x / 2.f, 0.f });
        label.setPosition({ WINDOW_WIDTH / 2.f, yPos });
        window.draw(label);

        RectangleShape inputBox({ 300.f, 40.f });
        inputBox.setPosition({ WINDOW_WIDTH / 2.f - 150.f, yPos + 35.f });
        inputBox.setFillColor(Color(30, 30, 45));
        inputBox.setOutlineThickness(2.f);
        inputBox.setOutlineColor((activeNameField == i) ? playerColors[i] : Color(60, 60, 80));
        window.draw(inputBox);

        char displayStr[51];
        textCopy(displayStr, names[i], 51);
        if (activeNameField == i) {
            float cursorBlink = fmod(menuHoverTimer, 1.0f);
            if (cursorBlink < 0.5f) textAppend(displayStr, "_", 51);
        }

        Text nameText(font, displayStr, 20);
        nameText.setFillColor(textColor);
        nameText.setPosition({ WINDOW_WIDTH / 2.f - 140.f, yPos + 42.f });
        window.draw(nameText);
    }

    const char* activeLabel = (activeNameField == 0) ? "Type Player 1's name, then press Enter"
        : "Type Player 2's name, then press Enter";
    Text activeHint(font, activeLabel, 14);
    activeHint.setFillColor(dimTextColor);
    FloatRect ahBounds = activeHint.getLocalBounds();
    activeHint.setOrigin({ ahBounds.size.x / 2.f, 0.f });
    activeHint.setPosition({ WINDOW_WIDTH / 2.f, 510.f });
    window.draw(activeHint);

    Text escHint(font, "Press ESC to go back", 12);
    escHint.setFillColor(Color(80, 80, 110));
    FloatRect escBounds = escHint.getLocalBounds();
    escHint.setOrigin({ escBounds.size.x / 2.f, 0.f });
    escHint.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 40.f });
    window.draw(escHint);
}

void SnakeGame::renderCountdown() {
    if (!fontLoaded) return;

    renderPlaying();

    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(10, 10, 18, 180));
    window.draw(overlay);

    char countStr[16];
    if (countdownValue > 0) intToString(countdownValue, countStr, 16);
    else textCopy(countStr, "GO!", 16);
    Text countText(font, countStr, 100);
    countText.setFillColor(accentColor);
    countText.setStyle(Text::Bold);
    FloatRect cBounds = countText.getLocalBounds();
    countText.setOrigin({ cBounds.size.x / 2.f, cBounds.size.y / 2.f });
    countText.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
    window.draw(countText);
}

void SnakeGame::renderPlaying() {
    if (!fontLoaded) return;

    // ── Top Bar ──
    RectangleShape topBar({ (float)WINDOW_WIDTH, (float)TOP_BAR_HEIGHT });
    topBar.setFillColor(uiPanelColor);
    window.draw(topBar);

    RectangleShape divider({ (float)WINDOW_WIDTH, 2.f });
    divider.setPosition({ 0.f, (float)(TOP_BAR_HEIGHT - 2) });
    divider.setFillColor(accentColor);
    window.draw(divider);

    // Player 1 info
    CircleShape dot1(6.f);
    dot1.setFillColor(snake1HeadColor);
    dot1.setPosition({ 15.f, 15.f });
    window.draw(dot1);

    Text p1Name(font, name1, 16);
    p1Name.setFillColor(snake1HeadColor);
    p1Name.setPosition({ 35.f, 10.f });
    window.draw(p1Name);

    char p1ScoreText[32];
    char tempNum[16];
    textCopy(p1ScoreText, "Score: ", 32);
    intToString(score1, tempNum, 16);
    textAppend(p1ScoreText, tempNum, 32);
    Text p1Score(font, p1ScoreText, 22);
    p1Score.setFillColor(textColor);
    p1Score.setStyle(Text::Bold);
    p1Score.setPosition({ 35.f, 35.f });
    window.draw(p1Score);

    // Player 2 info
    CircleShape dot2(6.f);
    dot2.setFillColor(snake2HeadColor);
    dot2.setPosition({ WINDOW_WIDTH - 195.f, 15.f });
    window.draw(dot2);

    Text p2Name(font, name2, 16);
    p2Name.setFillColor(snake2HeadColor);
    p2Name.setPosition({ WINDOW_WIDTH - 175.f, 10.f });
    window.draw(p2Name);

    char p2ScoreText[32];
    textCopy(p2ScoreText, "Score: ", 32);
    intToString(score2, tempNum, 16);
    textAppend(p2ScoreText, tempNum, 32);
    Text p2Score(font, p2ScoreText, 22);
    p2Score.setFillColor(textColor);
    p2Score.setStyle(Text::Bold);
    p2Score.setPosition({ WINDOW_WIDTH - 175.f, 35.f });
    window.draw(p2Score);

    // Pause hint
    Text pauseHint(font, "[ESC] Pause", 11);
    pauseHint.setFillColor(dimTextColor);
    FloatRect phBounds = pauseHint.getLocalBounds();
    pauseHint.setOrigin({ phBounds.size.x / 2.f, phBounds.size.y / 2.f });
    pauseHint.setPosition({ WINDOW_WIDTH / 2.f, 20.f });
    window.draw(pauseHint);

    // Speed indicator
    int speedPercent = static_cast<int>(((0.15f - tickDelay) / (0.15f - minTickDelay)) * 100.f);
    if (speedPercent < 0) speedPercent = 0;
    char speedBuffer[32];
    textCopy(speedBuffer, "Speed: ", 32);
    intToString(speedPercent, tempNum, 16);
    textAppend(speedBuffer, tempNum, 32);
    textAppend(speedBuffer, "%", 32);
    Text speedText(font, speedBuffer, 11);
    speedText.setFillColor(dimTextColor);
    FloatRect spBounds = speedText.getLocalBounds();
    speedText.setOrigin({ spBounds.size.x / 2.f, spBounds.size.y / 2.f });
    speedText.setPosition({ WINDOW_WIDTH / 2.f, 42.f });
    window.draw(speedText);

    // ── Grid ──
    RectangleShape gridBg(Vector2f(WINDOW_WIDTH, GRID_ROWS * CELL_SIZE));
    gridBg.setPosition({ 0.f, (float)TOP_BAR_HEIGHT });
    gridBg.setFillColor(bgColor);
    window.draw(gridBg);

    for (int x = 0; x <= GRID_COLS; x++) {
        RectangleShape line({ 1.f, (float)(GRID_ROWS * CELL_SIZE) });
        line.setPosition({ (float)(x * CELL_SIZE), (float)TOP_BAR_HEIGHT });
        line.setFillColor(gridLineColor);
        window.draw(line);
    }
    for (int y = 0; y <= GRID_ROWS; y++) {
        RectangleShape line({ (float)(GRID_COLS * CELL_SIZE), 1.f });
        line.setPosition({ 0.f, (float)(TOP_BAR_HEIGHT + y * CELL_SIZE) });
        line.setFillColor(gridLineColor);
        window.draw(line);
    }

    // ── Food ──
    float pulse = 1.0f + 0.15f * static_cast<float>(sin(foodPulseTimer * 4.f));
    if (appleTextureLoaded && appleSprite) {
        float baseScaleX = (float)CELL_SIZE / appleTexture.getSize().x;
        float baseScaleY = (float)CELL_SIZE / appleTexture.getSize().y;
        appleSprite->setScale({ baseScaleX * pulse, baseScaleY * pulse });
        float offsetX = (CELL_SIZE - CELL_SIZE * pulse) / 2.f;
        float offsetY = (CELL_SIZE - CELL_SIZE * pulse) / 2.f;
        appleSprite->setPosition({ foodPos.x * CELL_SIZE + offsetX, TOP_BAR_HEIGHT + foodPos.y * CELL_SIZE + offsetY });
        window.draw(*appleSprite);
    }
    else {
        float radius = (CELL_SIZE / 2.f) * pulse;
        CircleShape foodCircle(radius);
        foodCircle.setFillColor(foodColor);
        foodCircle.setOrigin({ radius, radius });
        foodCircle.setPosition({ foodPos.x * CELL_SIZE + CELL_SIZE / 2.f,
                               TOP_BAR_HEIGHT + foodPos.y * CELL_SIZE + CELL_SIZE / 2.f });
        window.draw(foodCircle);
    }

    // ── Snake 1 ──
    for (int i = 0; i < snake1Length; i++) {
        RectangleShape seg({ CELL_SIZE - 2.f, CELL_SIZE - 2.f });
        seg.setPosition({ snake1[i].x * CELL_SIZE + 1.f, TOP_BAR_HEIGHT + snake1[i].y * CELL_SIZE + 1.f });

        if (i == 0) {
            seg.setFillColor(snake1HeadColor);
            window.draw(seg);
            // Eyes
            float eyeR = 3.f;
            CircleShape eye(eyeR);
            eye.setFillColor(Color::White);
            float headCX = snake1[i].x * CELL_SIZE + CELL_SIZE / 2.f;
            float headCY = TOP_BAR_HEIGHT + snake1[i].y * CELL_SIZE + CELL_SIZE / 2.f;
            float ex1 = headCX - 5.f, ey1 = headCY - 5.f;
            float ex2 = headCX + 2.f, ey2 = headCY - 5.f;
            if (dir1 == Direction::DIR_UP) { ex1 = headCX - 6.f; ey1 = headCY - 5.f; ex2 = headCX + 3.f; ey2 = headCY - 5.f; }
            if (dir1 == Direction::DIR_DOWN) { ex1 = headCX - 6.f; ey1 = headCY + 2.f;  ex2 = headCX + 3.f; ey2 = headCY + 2.f; }
            if (dir1 == Direction::DIR_LEFT) { ex1 = headCX - 6.f; ey1 = headCY - 6.f; ex2 = headCX - 6.f; ey2 = headCY + 3.f; }
            if (dir1 == Direction::DIR_RIGHT) { ex1 = headCX + 3.f; ey1 = headCY - 6.f; ex2 = headCX + 3.f; ey2 = headCY + 3.f; }
            eye.setPosition({ ex1, ey1 });
            window.draw(eye);
            eye.setPosition({ ex2, ey2 });
            window.draw(eye);
        }
        else {
            float t = static_cast<float>(i) / static_cast<float>(snake1Length);
            uint8_t r = static_cast<uint8_t>(lerp(snake1BodyColor.r, 40.f, t * 0.5f));
            uint8_t g = static_cast<uint8_t>(lerp(snake1BodyColor.g, 40.f, t * 0.5f));
            uint8_t b = static_cast<uint8_t>(lerp(snake1BodyColor.b, 40.f, t * 0.5f));
            seg.setFillColor(Color(r, g, b));
            window.draw(seg);
        }
    }

    // ── Snake 2 ──
    for (int i = 0; i < snake2Length; i++) {
        RectangleShape seg({ CELL_SIZE - 2.f, CELL_SIZE - 2.f });
        seg.setPosition({ snake2[i].x * CELL_SIZE + 1.f, TOP_BAR_HEIGHT + snake2[i].y * CELL_SIZE + 1.f });

        if (i == 0) {
            seg.setFillColor(snake2HeadColor);
            window.draw(seg);
            float eyeR = 3.f;
            CircleShape eye(eyeR);
            eye.setFillColor(Color::White);
            float headCX = snake2[i].x * CELL_SIZE + CELL_SIZE / 2.f;
            float headCY = TOP_BAR_HEIGHT + snake2[i].y * CELL_SIZE + CELL_SIZE / 2.f;
            float ex1 = headCX - 5.f, ey1 = headCY - 5.f;
            float ex2 = headCX + 2.f, ey2 = headCY - 5.f;
            if (dir2 == Direction::DIR_UP) { ex1 = headCX - 6.f; ey1 = headCY - 5.f; ex2 = headCX + 3.f; ey2 = headCY - 5.f; }
            if (dir2 == Direction::DIR_DOWN) { ex1 = headCX - 6.f; ey1 = headCY + 2.f;  ex2 = headCX + 3.f; ey2 = headCY + 2.f; }
            if (dir2 == Direction::DIR_LEFT) { ex1 = headCX - 6.f; ey1 = headCY - 6.f; ex2 = headCX - 6.f; ey2 = headCY + 3.f; }
            if (dir2 == Direction::DIR_RIGHT) { ex1 = headCX + 3.f; ey1 = headCY - 6.f; ex2 = headCX + 3.f; ey2 = headCY + 3.f; }
            eye.setPosition({ ex1, ey1 });
            window.draw(eye);
            eye.setPosition({ ex2, ey2 });
            window.draw(eye);
        }
        else {
            float t = static_cast<float>(i) / static_cast<float>(snake2Length);
            uint8_t r = static_cast<uint8_t>(lerp(snake2BodyColor.r, 40.f, t * 0.5f));
            uint8_t g = static_cast<uint8_t>(lerp(snake2BodyColor.g, 40.f, t * 0.5f));
            uint8_t b = static_cast<uint8_t>(lerp(snake2BodyColor.b, 40.f, t * 0.5f));
            seg.setFillColor(Color(r, g, b));
            window.draw(seg);
        }
    }

    // ── Particles ──
    for (int i = 0; i < particleCount; i++) {
        float alpha = (particles[i].life / 0.6f) * 255.f;
        if (alpha > 255.f) alpha = 255.f;
        if (alpha < 0.f) alpha = 0.f;
        CircleShape dot(3.f);
        dot.setFillColor(Color(particles[i].color.r, particles[i].color.g,
            particles[i].color.b, static_cast<uint8_t>(alpha)));
        dot.setPosition(particles[i].pos);
        window.draw(dot);
    }
}

void SnakeGame::renderPaused() {
    if (!fontLoaded) return;

    renderPlaying();

    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(10, 10, 18, 200));
    window.draw(overlay);

    Text pauseText(font, "PAUSED", 52);
    pauseText.setFillColor(accentColor);
    pauseText.setStyle(Text::Bold);
    FloatRect pBounds = pauseText.getLocalBounds();
    pauseText.setOrigin({ pBounds.size.x / 2.f, pBounds.size.y / 2.f });
    pauseText.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 40.f });
    window.draw(pauseText);

    Text resumeHint(font, "Press ESC or P to Resume", 16);
    resumeHint.setFillColor(dimTextColor);
    FloatRect rBounds = resumeHint.getLocalBounds();
    resumeHint.setOrigin({ rBounds.size.x / 2.f, rBounds.size.y / 2.f });
    resumeHint.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 20.f });
    window.draw(resumeHint);

    Text quitHint(font, "Press Q to Quit to Menu", 14);
    quitHint.setFillColor(Color(80, 80, 110));
    FloatRect qBounds = quitHint.getLocalBounds();
    quitHint.setOrigin({ qBounds.size.x / 2.f, qBounds.size.y / 2.f });
    quitHint.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 50.f });
    window.draw(quitHint);
}

void SnakeGame::renderGameOver() {
    if (!fontLoaded) return;

    renderPlaying();

    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(10, 10, 18, 210));
    window.draw(overlay);

    Text goTitle(font, "GAME OVER", 52);
    goTitle.setFillColor(Color(255, 60, 80));
    goTitle.setStyle(Text::Bold);
    FloatRect goBounds = goTitle.getLocalBounds();
    goTitle.setOrigin({ goBounds.size.x / 2.f, goBounds.size.y / 2.f });
    goTitle.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 130.f });
    window.draw(goTitle);

    Text resultText(font, resultMessage, 24);
    resultText.setFillColor(accentColor);
    FloatRect resBounds = resultText.getLocalBounds();
    resultText.setOrigin({ resBounds.size.x / 2.f, resBounds.size.y / 2.f });
    resultText.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 70.f });
    window.draw(resultText);

    char scoreStr[160];
    char numBuf[16];
    textCopy(scoreStr, name1, 160);
    textAppend(scoreStr, ": ", 160);
    intToString(score1, numBuf, 16);
    textAppend(scoreStr, numBuf, 160);
    textAppend(scoreStr, "    ", 160);
    textAppend(scoreStr, name2, 160);
    textAppend(scoreStr, ": ", 160);
    intToString(score2, numBuf, 16);
    textAppend(scoreStr, numBuf, 160);
    Text scoresText(font, scoreStr, 18);
    scoresText.setFillColor(textColor);
    FloatRect scBounds = scoresText.getLocalBounds();
    scoresText.setOrigin({ scBounds.size.x / 2.f, scBounds.size.y / 2.f });
    scoresText.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 30.f });
    window.draw(scoresText);

    // Buttons
    const char* options[] = { "Play Again", "Main Menu", "Exit" };
    float btnX = WINDOW_WIDTH / 2.f - 120.f;
    float btnY = WINDOW_HEIGHT / 2.f + 40.f;
    float btnW = 240.f;
    float btnH = 45.f;
    float gap = 12.f;

    Vector2i mousePos = Mouse::getPosition(window);

    for (int i = 0; i < 3; i++) {
        FloatRect btnRect({ btnX, btnY + i * (btnH + gap) }, { btnW, btnH });
        bool hovered = btnRect.contains(Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));
        bool selected = (gameOverSelection == i);

        RectangleShape btn({ btnW, btnH });
        btn.setPosition({ btnRect.position.x, btnRect.position.y });

        if (selected || hovered) {
            btn.setFillColor(accentColor);
            if (hovered) gameOverSelection = i;
        }
        else {
            btn.setFillColor(Color(35, 35, 50));
            btn.setOutlineColor(Color(60, 60, 80));
            btn.setOutlineThickness(1.f);
        }
        window.draw(btn);

        Text btnText(font, options[i], 18);
        btnText.setFillColor(textColor);
        FloatRect textBounds = btnText.getLocalBounds();
        btnText.setOrigin({ textBounds.size.x / 2.f, textBounds.size.y / 2.f });
        btnText.setPosition({ btnRect.position.x + btnW / 2.f, btnRect.position.y + btnH / 2.f - 2.f });
        window.draw(btnText);
    }
}

void SnakeGame::renderLeaderboard() {
    if (!fontLoaded) return;

    RectangleShape bg(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    bg.setFillColor(Color(12, 12, 20));
    window.draw(bg);

    Text title(font, "LEADERBOARD", 40);
    title.setFillColor(accentColor);
    title.setStyle(Text::Bold);
    FloatRect tBounds = title.getLocalBounds();
    title.setOrigin({ tBounds.size.x / 2.f, tBounds.size.y / 2.f });
    title.setPosition({ WINDOW_WIDTH / 2.f, 50.f });
    window.draw(title);

    Text subtitle(font, "Snake - Highest Scores", 16);
    subtitle.setFillColor(dimTextColor);
    FloatRect sBounds = subtitle.getLocalBounds();
    subtitle.setOrigin({ sBounds.size.x / 2.f, sBounds.size.y / 2.f });
    subtitle.setPosition({ WINDOW_WIDTH / 2.f, 90.f });
    window.draw(subtitle);

    RectangleShape line({ 500.f, 2.f });
    line.setPosition({ WINDOW_WIDTH / 2.f - 250.f, 115.f });
    line.setFillColor(accentColor);
    window.draw(line);

    // Column headers
    Text rankHeader(font, "RANK", 14);
    rankHeader.setFillColor(dimTextColor);
    rankHeader.setPosition({ 100.f, 135.f });
    window.draw(rankHeader);

    Text nameHeader(font, "PLAYER", 14);
    nameHeader.setFillColor(dimTextColor);
    nameHeader.setPosition({ 220.f, 135.f });
    window.draw(nameHeader);

    Text scoreHeader(font, "SCORE", 14);
    scoreHeader.setFillColor(dimTextColor);
    scoreHeader.setPosition({ 500.f, 135.f });
    window.draw(scoreHeader);

    // Get records
    int snakeRecordCount = 0;
    LeaderboardEntry* snakeRecords = leaderboard.getRecordsByGame("Snake", snakeRecordCount);

    int maxDisplay = 10;
    if (snakeRecordCount < maxDisplay) maxDisplay = snakeRecordCount;

    if (maxDisplay == 0) {
        Text noData(font, "No records yet. Play a game first!", 18);
        noData.setFillColor(dimTextColor);
        FloatRect nBounds = noData.getLocalBounds();
        noData.setOrigin({ nBounds.size.x / 2.f, nBounds.size.y / 2.f });
        noData.setPosition({ WINDOW_WIDTH / 2.f, 300.f });
        window.draw(noData);
    }

    for (int i = 0; i < maxDisplay; i++) {
        float yPos = 170.f + i * 40.f;

        RectangleShape rowBg({ 500.f, 35.f });
        rowBg.setPosition({ WINDOW_WIDTH / 2.f - 250.f, yPos });
        rowBg.setFillColor((i % 2 == 0) ? Color(20, 20, 32) : Color(28, 28, 42));
        window.draw(rowBg);

        char rankStr[16];
        Color rankColor = textColor;
        if (i == 0) { textCopy(rankStr, "1st", 16); rankColor = Color(255, 215, 0); }
        else if (i == 1) { textCopy(rankStr, "2nd", 16); rankColor = Color(192, 192, 192); }
        else if (i == 2) { textCopy(rankStr, "3rd", 16); rankColor = Color(205, 127, 50); }
        else {
            intToString(i + 1, rankStr, 16);
            textAppend(rankStr, "th", 16);
        }

        Text rankText(font, rankStr, 18);
        rankText.setFillColor(rankColor);
        if (i < 3) rankText.setStyle(Text::Bold);
        rankText.setPosition({ 115.f, yPos + 6.f });
        window.draw(rankText);

        Text nameText(font, snakeRecords[i].playerName, 18);
        nameText.setFillColor(textColor);
        nameText.setPosition({ 220.f, yPos + 6.f });
        window.draw(nameText);

        char scoreNum[16];
        intToString(snakeRecords[i].score, scoreNum, 16);
        Text scoreText(font, scoreNum, 18);
        scoreText.setFillColor(accentColor);
        scoreText.setStyle(Text::Bold);
        scoreText.setPosition({ 510.f, yPos + 6.f });
        window.draw(scoreText);
    }

    // Clean up dynamic allocation
    if (snakeRecords) {
        delete[] snakeRecords;
        snakeRecords = nullptr;
    }

    Text backHint(font, "Press ESC or ENTER or Click anywhere to go back", 13);
    backHint.setFillColor(dimTextColor);
    FloatRect bBounds = backHint.getLocalBounds();
    backHint.setOrigin({ bBounds.size.x / 2.f, 0.f });
    backHint.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 40.f });
    window.draw(backHint);
}

// ═══════════════════════════════════════════════
//  HELPERS
// ═══════════════════════════════════════════════

void SnakeGame::spawnParticles(Vector2f pos, Color color, int count) {
    for (int i = 0; i < count; i++) {
        if (particleCount >= MAX_PARTICLES) break;
        Particle& p = particles[particleCount];
        p.pos = pos;
        float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.f;
        float speed = 50.f + static_cast<float>(rand() % 100);
        p.vel = Vector2f(static_cast<float>(cos(angle) * speed), static_cast<float>(sin(angle) * speed));
        p.life = 0.3f + static_cast<float>(rand() % 30) / 100.f;
        p.color = color;
        particleCount++;
    }
}

float SnakeGame::lerp(float a, float b, float t) {
    if (t < 0.f) t = 0.f;
    if (t > 1.f) t = 1.f;
    return a + (b - a) * t;
}

void SnakeGame::intToString(int value, char* outText, int maxLen) {
    if (maxLen <= 1) return;
    if (value == 0) {
        outText[0] = '0';
        outText[1] = '\0';
        return;
    }

    bool negative = false;
    if (value < 0) {
        negative = true;
        value = -value;
    }

    char rev[32];
    int idx = 0;
    while (value > 0 && idx < 31) {
        rev[idx] = static_cast<char>('0' + (value % 10));
        value /= 10;
        idx++;
    }

    int outIdx = 0;
    if (negative && outIdx < maxLen - 1) {
        outText[outIdx++] = '-';
    }

    for (int i = idx - 1; i >= 0 && outIdx < maxLen - 1; i--) {
        outText[outIdx++] = rev[i];
    }
    outText[outIdx] = '\0';
}

int SnakeGame::textLength(const char* text) {
    int len = 0;
    while (text[len] != '\0') len++;
    return len;
}

void SnakeGame::textCopy(char* dst, const char* src, int maxLen) {
    int i = 0;
    while (src[i] != '\0' && i < maxLen - 1) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

void SnakeGame::textAppend(char* dst, const char* src, int maxLen) {
    int start = textLength(dst);
    int i = 0;
    while (src[i] != '\0' && start + i < maxLen - 1) {
        dst[start + i] = src[i];
        i++;
    }
    dst[start + i] = '\0';
}