#pragma once

#include "Leaderboard.h"
#include "TicTacToe.h"
#include "Snake_N_Ladder.h"
#include "Rock_Paper_Scissors.h"
#include "Match_Cards.h"
#include "Number_Guessing_Game.h"

const int TOTAL_GAMES = 5;

class Hub {
private:
    LeaderboardManager  leaderboard;
    TicTacToe ticTacToe;
    SnakeAndLadder snakeAndLadder;
    RockPaperScissors rockPaperScissors;
    MatchCards matchCards;
    NumberGuessingGame numberGuessingGame;
    bool running;

    void clearScreen() const;
    void printBanner() const;
    void printMainMenu() const;
    void printModeMenu() const;
    void pause() const;
    int  getInput(int low, int high) const;
    void readName(char* buf, int maxLen) const;
    void launchGame(int gameChoice);
    void showLeaderboardMenu() const;

public:
    Hub();
    ~Hub();

    void run();
};