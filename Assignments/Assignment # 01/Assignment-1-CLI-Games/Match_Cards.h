#pragma once

const int GRID_ROWS = 4;
const int GRID_COLS = 4;
const int TOTAL_CARDS = GRID_ROWS * GRID_COLS;

class MatchCards {
private:
    int  cards[TOTAL_CARDS];
    bool revealed[TOTAL_CARDS];
    int  scores[2];
    int  totalMatched;

    void shuffleDeck();
    void printBoard(int flipA, int flipB) const;
    void printCardValue(int i, int flipA, int flipB) const;
    bool allMatched() const;
    int  getInput(int low, int high) const;
    void pause() const;
    void computerPickTwo(int& pick1, int& pick2) const;

public:
    MatchCards();
    ~MatchCards();

    void reset();
    void play(const char* p1, const char* p2, bool vsComputer, int& score1, int& score2);
};