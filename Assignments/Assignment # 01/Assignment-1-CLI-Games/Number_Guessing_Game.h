#pragma once

const int MIN = 1;
const int MAX = 100;
const int MAX_ATTEMPTS = 7;
const int ROUNDS = 3;

class NumberGuessingGame {
private:
    int  secretNumber;
    int  attemptsLeft;
    int  wins[2];

    int  generateSecretNumber() const;
    int  getInput(int low, int high) const;
    void printAttempt() const;

    int  playerRound(const char* name, bool isComputer);
    int  calcScore(int attemptsUsed) const;

public:
    NumberGuessingGame();
    ~NumberGuessingGame();

    void reset();
    void play(const char* p1, const char* p2, bool vsComputer, int& score1, int& score2);
};