#pragma once

class RockPaperScissors {
private:
    int  roundsToPlay;
    int  wins1, wins2, draws;

    int  getInput(int lo, int hi) const;
    int  getPlayerChoice(const char* name) const;
    int  getComputerChoice() const;
    void printChoice(int choice) const;
    int  resolveRound(int c1, int c2) const;
    void printRoundResult(int result, const char* p1, const char* p2) const;

public:
    RockPaperScissors();
    ~RockPaperScissors();

    void reset();
    void play(const char* p1, const char* p2, bool vsComputer, int& score1, int& score2);
};