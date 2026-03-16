#pragma once

const int TTT_SIZE = 3;

class TicTacToe {
private:
    char board[TTT_SIZE][TTT_SIZE];
    char currentPlayer;
    int  moveCount;

    void Board();
    void printBoard() const;
    bool isCellValid(int row, int col) const;
    bool isBoardFull() const;
    bool checkWin(char symbol) const;

    void playerMove(const char* name);
    void computerMove();

    int getInput(int low, int high) const;

public:
    TicTacToe();
    ~TicTacToe();

    void reset();
    void play(const char* p1, const char* p2, bool vsComputer, int& score1, int& score2);
};