#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include "LeaderboardManager.h"

using namespace std;
using namespace sf;

struct GameState {
    static const int STATE_MENU = 0;
    static const int STATE_NAME_INPUT = 1;
    static const int STATE_PLAYING = 2;
    static const int STATE_PAUSED = 3;
    static const int STATE_GAME_OVER = 4;
    static const int STATE_LEADERBOARD = 5;
    static const int STATE_COUNTDOWN = 6;
    static const int STATE_EXIT = 7;
};

struct Direction {
    static const int DIR_UP = 0;
    static const int DIR_DOWN = 1;
    static const int DIR_LEFT = 2;
    static const int DIR_RIGHT = 3;
};

// Particle struct
struct Particle {
    Vector2f pos;
    Vector2f vel;
    float life;
    Color color;
};

class SnakeGame {
private:
    // ──────── Constants ────────
    static const int CELL_SIZE = 30;
    static const int GRID_COLS = 24;
    static const int GRID_ROWS = 20;
    static const int TOP_BAR_HEIGHT = 80;
    static const int WINDOW_WIDTH = CELL_SIZE * GRID_COLS;
    static const int WINDOW_HEIGHT = CELL_SIZE * GRID_ROWS + TOP_BAR_HEIGHT;
    static const int MAX_SNAKE_LENGTH = 200;
    static const int MAX_PARTICLES = 300;

    // ──────── Window ────────
    RenderWindow window;

    // ──────── Assets ────────
    Font font;
    bool fontLoaded;
    Texture snakeLogoTexture;
    Sprite* snakeLogoSprite;
    bool logoLoaded;
    Texture appleTexture;
    Sprite* appleSprite;
    bool appleTextureLoaded;

    // ──────── Audio ────────
    Music bgm;
    SoundBuffer* eatBuffer;
    SoundBuffer* gameOverBuffer;
    SoundBuffer* clickBuffer;
    Sound* eatSound;
    Sound* gameOverSound;
    Sound* clickSound;
    bool eatLoaded, gameOverLoaded, clickLoaded;

    // ──────── Snake 1 (WASD) ────────
    Vector2i snake1[MAX_SNAKE_LENGTH];
    int snake1Length;
    int dir1;
    int lastDir1;
    int score1;
    char name1[50];

    // ──────── Snake 2 (Arrow Keys) ────────
    Vector2i snake2[MAX_SNAKE_LENGTH];
    int snake2Length;
    int dir2;
    int lastDir2;
    int score2;
    char name2[50];

    // ──────── Food ────────
    Vector2i foodPos;

    // ──────── Game Logic ────────
    int state;
    Clock gameClock;
    float tickTimer;
    float tickDelay;
    float minTickDelay;

    // ──────── Name Input ────────
    int activeNameField;
    char inputName1[50], inputName2[50];

    // ──────── Countdown ────────
    Clock countdownClock;
    int countdownValue;

    // ──────── Game Over ────────
    char resultMessage[120];
    bool snake1Dead, snake2Dead;

    // ──────── Leaderboard ────────
    LeaderboardManager leaderboard;

    // ──────── Visual ────────
    float foodPulseTimer;
    float menuHoverTimer;
    int menuSelection;
    int gameOverSelection;

    // ──────── Particles ────────
    Particle particles[MAX_PARTICLES];
    int particleCount;

    // ──────── Colors ────────
    Color bgColor;
    Color gridLineColor;
    Color snake1HeadColor, snake1BodyColor;
    Color snake2HeadColor, snake2BodyColor;
    Color foodColor;
    Color uiPanelColor;
    Color accentColor;
    Color textColor;
    Color dimTextColor;

public:
    SnakeGame();
    ~SnakeGame();
    void run();

private:
    void loadAssets();
    void initColors();
    void resetGame();
    void spawnFood();

    void handleEvents();
    void update(float dt);
    void render();

    void handleMenuEvent(const Event& e);
    void handleNameInputEvent(const Event& e);
    void handlePlayingEvent(const Event& e);
    void handlePausedEvent(const Event& e);
    void handleGameOverEvent(const Event& e);
    void handleLeaderboardEvent(const Event& e);

    void updatePlaying(float dt);
    void updateCountdown(float dt);
    void updateParticles(float dt);

    void renderMenu();
    void renderNameInput();
    void renderCountdown();
    void renderPlaying();
    void renderPaused();
    void renderGameOver();
    void renderLeaderboard();

    void moveSnake(Vector2i* snake, int length, int dir);
    bool checkWallCollision(const Vector2i& head);
    bool checkSelfCollision(const Vector2i* snake, int length);
    bool checkBodyCollision(const Vector2i& head, const Vector2i* otherSnake, int otherLength);
    void checkCollisions();
    void determineResult();

    void spawnParticles(Vector2f pos, Color color, int count);
    float lerp(float a, float b, float t);
    void intToString(int value, char* outText, int maxLen);
    int textLength(const char* text);
    void textCopy(char* dst, const char* src, int maxLen);
    void textAppend(char* dst, const char* src, int maxLen);
};