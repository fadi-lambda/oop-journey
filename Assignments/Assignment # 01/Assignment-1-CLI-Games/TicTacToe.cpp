#include "TicTacToe.h"
#include "Colors.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

TicTacToe::TicTacToe() {
    srand(time(0));
    currentPlayer = 'X';
    moveCount = 0;

    for (int r = 0; r < TTT_SIZE; r++) {
        for (int c = 0; c < TTT_SIZE; c++) {
            board[r][c] = ' ';
        }
    }
}

TicTacToe::~TicTacToe() {}

void TicTacToe::Board() {
    for (int r = 0; r < TTT_SIZE; r++) {
        for (int c = 0; c < TTT_SIZE; c++) {
            board[r][c] = ' ';
        }
    }
    currentPlayer = 'X';
    moveCount = 0;
}

void TicTacToe::printBoard() const {
    cout << endl;
    set_color(Cyan, Black);
    cout << "     1   2   3 " << endl;
    cout << "   =============" << endl;
    for (int r = 0; r < TTT_SIZE; r++) {
        set_color(Cyan, Black);
        cout << " " << (r + 1) << " | ";
        for (int c = 0; c < TTT_SIZE; c++) {
            if (board[r][c] == 'X') {
                set_color(Yellow, Black);
            }
            else if (board[r][c] == 'O') {
                set_color(Pink, Black);
            }
            else {
                set_color(White, Black);
            }
            cout << board[r][c];
            set_color(Cyan, Black);
            if (c < TTT_SIZE - 1) {
                cout << " | ";
            }
        }
        set_color(Cyan, Black);
        cout << " |" << endl;
        cout << "   =============" << endl;
    }
    set_color(White, Black);
    cout << endl;
}

bool TicTacToe::isCellValid(int row, int col) const {
    if (row < 0 || row >= TTT_SIZE) {
        return false;
    }
    if (col < 0 || col >= TTT_SIZE) {
        return false;
    }
    return (board[row][col] == ' ');
}

bool TicTacToe::isBoardFull() const {
    return (moveCount == TTT_SIZE * TTT_SIZE);
}

bool TicTacToe::checkWin(char symbol) const {
    for (int r = 0; r < TTT_SIZE; r++) {
        bool win = true;
        for (int c = 0; c < TTT_SIZE; c++) {
            if (board[r][c] != symbol) {
                win = false;
            }
        }
        if (win) {
            return true;
        }
    }

    for (int c = 0; c < TTT_SIZE; c++) {
        bool win = true;
        for (int r = 0; r < TTT_SIZE; r++) {
            if (board[r][c] != symbol) {
                win = false;
            }
        }
        if (win) {
            return true;
        }
    }

    bool win = true;
    for (int i = 0; i < TTT_SIZE; i++) {
        if (board[i][i] != symbol) {
            win = false;
        }
    }
    if (win) {
        return true;
    }

    win = true;
    for (int i = 0; i < TTT_SIZE; i++) {
        if (board[i][TTT_SIZE - 1 - i] != symbol) {
            win = false;
        }
    }

    if (win) {
        return true;
    }

    return false;
}

int TicTacToe::getInput(int low, int high) const {
    int val;
    while (true) {
        cin >> val;
        if (cin.fail() || val < low || val > high) {
            cin.clear();
            cin.ignore();
            set_color(Red, Black);
            cout << "Invalid! Enter a number between " << low << " and " << high << ": ";
            set_color(White, Black);
        }
        else {
            cin.ignore();
            return val;
        }
    }
}

void TicTacToe::playerMove(const char* name) {
    int row, col;
    set_color(Yellow, Black);
    cout << name << " (" << currentPlayer << ") >>> enter row (1-3): ";
    set_color(White, Black);
    row = getInput(1, 3) - 1;
    set_color(Yellow, Black);
    cout << name << " (" << currentPlayer << ") >>> enter col (1-3): ";
    set_color(White, Black);
    col = getInput(1, 3) - 1;

    while (!isCellValid(row, col)) {
        set_color(Red, Black);
        cout << "Cell already taken." << endl;
        set_color(Yellow, Black);
        cout << "Enter row (1-3): ";
        set_color(White, Black);
        row = getInput(1, 3) - 1;
        set_color(Yellow, Black);
        cout << "Enter col (1-3): ";
        set_color(White, Black);
        col = getInput(1, 3) - 1;
    }

    board[row][col] = currentPlayer;
    moveCount++;
}

void TicTacToe::computerMove() {
    for (int r = 0; r < TTT_SIZE; r++) {
        for (int c = 0; c < TTT_SIZE; c++) {
            if (board[r][c] == ' ') {
                board[r][c] = 'O';
                if (checkWin('O')) {
                    moveCount++;
                    set_color(Pink, Black);
                    cout << "Computer plays : row " << r + 1 << ", col " << c + 1 << endl;
                    set_color(White, Black);
                    return;
                }
                board[r][c] = ' ';
            }
        }
    }

    for (int r = 0; r < TTT_SIZE; r++) {
        for (int c = 0; c < TTT_SIZE; c++) {
            if (board[r][c] == ' ') {
                board[r][c] = 'X';
                if (checkWin('X')) {
                    board[r][c] = 'O';
                    moveCount++;
                    set_color(Pink, Black);
                    cout << "Computer plays: row " << r + 1 << ", col " << c + 1 << endl;
                    set_color(White, Black);
                    return;
                }
                board[r][c] = ' ';
            }
        }
    }

    if (board[1][1] == ' ') {
        board[1][1] = 'O';
        moveCount++;
        set_color(Pink, Black);
        cout << "Computer plays: row 2, col 2" << endl;
        set_color(White, Black);
        return;
    }

    int r, c;
    do {
        r = rand() % TTT_SIZE;
        c = rand() % TTT_SIZE;
    } while (board[r][c] != ' ');

    board[r][c] = 'O';
    moveCount++;
    set_color(Pink, Black);
    cout << "Computer plays: row " << r + 1 << ", col " << c + 1 << endl;
    set_color(White, Black);
}

void TicTacToe::reset() {
    Board();
}

void TicTacToe::play(const char* p1, const char* p2, bool vsComputer, int& score1, int& score2) {
    score1 = 0;
    score2 = 0;

    int wins1 = 0;
    int wins2 = 0;
    int draws = 0;

    const int ROUNDS = 3;

    set_color(Pink, Black);
    cout << "===== TIC TAC TOE =====" << endl;
    set_color(Yellow, Black);
    cout << p1 << " = X     " << p2 << " = O" << endl;
    set_color(White, Black);
    cout << "Best of " << ROUNDS << " rounds" << endl;

    for (int round = 1; round <= ROUNDS; round++) {
        Board();
        cout << endl;
        set_color(Cyan, Black);
        cout << ">>> Round " << round << " <<<" << endl;
        set_color(White, Black);

        bool roundOver = false;

        while (!roundOver) {
            printBoard();

            if (currentPlayer == 'X') {
                playerMove(p1);
            }
            else {
                if (vsComputer) {
                    computerMove();
                }
                else {
                    playerMove(p2);
                }
            }

            if (checkWin(currentPlayer)) {
                printBoard();
                if (currentPlayer == 'X') {
                    set_color(Yellow, Black);
                    cout << "  *** " << p1 << " wins this round! ***" << endl;
                    set_color(White, Black);
                    wins1++;
                }
                else {
                    set_color(Pink, Black);
                    cout << "  *** " << p2 << " wins this round! ***" << endl;
                    set_color(White, Black);
                    wins2++;
                }
                roundOver = true;
            }
            else if (isBoardFull()) {
                printBoard();
                set_color(Grey, Black);
                cout << "  *** Draw! ***" << endl;
                set_color(White, Black);
                draws++;
                roundOver = true;
            }
            else {
                if (currentPlayer == 'X') {
                    currentPlayer = 'O';
                }
                else {
                    currentPlayer = 'X';
                }
            }
        }

        set_color(Cyan, Black);
        cout << "Score --- " << p1 << ": " << wins1 << "   " << p2 << ": " << wins2 << "   Draws: " << draws << endl;
        set_color(White, Black);
    }

    cout << endl;
    set_color(Pink, Black);
    cout << "===== FINAL RESULT =====" << endl;
    set_color(White, Black);

    if (wins1 > wins2) {
        set_color(Yellow, Black);
        cout << p1 << " wins the match!" << endl;
        set_color(White, Black);
    }
    else if (wins2 > wins1) {
        set_color(Pink, Black);
        cout << p2 << " wins the match!" << endl;
        set_color(White, Black);
    }
    else {
        set_color(Grey, Black);
        cout << "Match drawn!" << endl;
        set_color(White, Black);
    }

    score1 = wins1;
    score2 = wins2;
}