#pragma once

const int BOARD_SIZE = 100;
const int NUM_SNAKES = 8;
const int NUM_LADDERS = 8;
const int MAX_PLAYERS = 2;

struct Snake {
    int head;
    int tail;
};

struct Ladder {
    int bottom;
    int top;
};

class SnakeAndLadder {
private:
    Snake snakes[NUM_SNAKES];
    Ladder ladders[NUM_LADDERS];
    int positions[MAX_PLAYERS];
    int numPlayers;

    void printBoard() const;
    void printPositions(const char* p1, const char* p2) const;
    int rollDice() const;
    int checkSnake(int pos) const;
    int checkLadder(int pos) const;
    int getInput(int low, int high) const;
    void pause() const;

public:
    SnakeAndLadder();
    ~SnakeAndLadder();

    void reset();
    void play(const char* p1, const char* p2, bool vsComputer, int& score1, int& score2);
};