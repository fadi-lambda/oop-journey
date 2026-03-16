#include "Snake_N_Ladder.h"
#include "Colors.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

SnakeAndLadder::SnakeAndLadder() {
    srand(time(0));
    numPlayers = 2;

    snakes[0].head = 97;
    snakes[0].tail = 78;

    snakes[1].head = 95;
    snakes[1].tail = 56;

    snakes[2].head = 88;
    snakes[2].tail = 24;

    snakes[3].head = 62;
    snakes[3].tail = 18;

    snakes[4].head = 49;
    snakes[4].tail = 11;

    snakes[5].head = 36;
    snakes[5].tail = 6;

    snakes[6].head = 32;
    snakes[6].tail = 10;

    snakes[7].head = 17;
    snakes[7].tail = 4;

    ladders[0].bottom = 1;
    ladders[0].top = 38;

    ladders[1].bottom = 4;
    ladders[1].top = 14;

    ladders[2].bottom = 9;
    ladders[2].top = 31;

    ladders[3].bottom = 20;
    ladders[3].top = 56;

    ladders[4].bottom = 28;
    ladders[4].top = 84;

    ladders[5].bottom = 40;
    ladders[5].top = 59;

    ladders[6].bottom = 51;
    ladders[6].top = 67;

    ladders[7].bottom = 63;
    ladders[7].top = 81;

    positions[0] = 0;
    positions[1] = 0;
}

SnakeAndLadder::~SnakeAndLadder() {}

void SnakeAndLadder::printBoard() const {
    set_color(Cyan, Black);
    cout << "==========================================================" << endl;

    for (int row = 9; row >= 0; row--) {
        cout << "  |";
        for (int col = 0; col < 10; col++) {
            int cellNum;
            if (row % 2 == 0) cellNum = row * 10 + col + 1;
            else cellNum = row * 10 + (9 - col) + 1;

            bool p1Here = (positions[0] == cellNum);
            bool p2Here = (positions[1] == cellNum);

            if (p1Here && p2Here) {
                set_color(Pink, Black);
                cout << " ** |";
            }
            else if (p1Here) {
                set_color(Yellow, Black);
                cout << " P1 |";
            }
            else if (p2Here) {
                set_color(Green, Black);
                cout << " P2 |";
            }
            else if (cellNum < 10) {
                set_color(Cyan, Black);
                cout << "  " << cellNum << " |";
            }
            else if (cellNum < 100) {
                set_color(Cyan, Black);
                cout << " " << cellNum << " |";
            }
            else {
                set_color(Cyan, Black);
                cout << " " << cellNum << "|";
            }
        }
        set_color(Cyan, Black);
        cout << endl;
        cout << "==========================================================" << endl;
    }
    set_color(White, Black);
    cout << endl;
}

void SnakeAndLadder::printPositions(const char* p1, const char* p2) const {
    set_color(Yellow, Black);
    cout << p1 << " is on square " << positions[0] << endl;
    set_color(Green, Black);
    cout << p2 << " is on square " << positions[1] << endl;
    set_color(White, Black);
}

int SnakeAndLadder::rollDice() const {
    return (rand() % 6) + 1;
}

int SnakeAndLadder::checkSnake(int pos) const {
    for (int i = 0; i < NUM_SNAKES; i++) {
        if (snakes[i].head == pos) {
            set_color(Red, Black);
            cout << "!!SNAKE at " << pos << "!Slide down to " << snakes[i].tail << " !!" << endl;
            set_color(White, Black);
            return snakes[i].tail;
        }
    }
    return pos;
}

int SnakeAndLadder::checkLadder(int pos) const {
    for (int i = 0; i < NUM_LADDERS; i++) {
        if (ladders[i].bottom == pos) {
            set_color(Green, Black);
            cout << ">> LADDER at " << pos << "! Climb up to " << ladders[i].top << " <<" << endl;
            set_color(White, Black);
            return ladders[i].top;
        }
    }
    return pos;
}

int SnakeAndLadder::getInput(int low, int high) const {
    int val;
    while (true) {
        cin >> val;
        if (val < low || val > high) {
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

void SnakeAndLadder::pause() const {
    set_color(Grey, Black);
    cout << "Press Enter to continue...";
    set_color(White, Black);
    cin.get();
}

void SnakeAndLadder::reset() {
    snakes[0].head = 97;
    snakes[0].tail = 78;

    snakes[1].head = 95;
    snakes[1].tail = 56;

    snakes[2].head = 88;
    snakes[2].tail = 24;

    snakes[3].head = 62;
    snakes[3].tail = 18;

    snakes[4].head = 49;
    snakes[4].tail = 11;

    snakes[5].head = 36;
    snakes[5].tail = 6;

    snakes[6].head = 32;
    snakes[6].tail = 10;

    snakes[7].head = 17;
    snakes[7].tail = 4;

    ladders[0].bottom = 1;
    ladders[0].top = 38;

    ladders[1].bottom = 4;
    ladders[1].top = 14;

    ladders[2].bottom = 9;
    ladders[2].top = 31;

    ladders[3].bottom = 20;
    ladders[3].top = 56;

    ladders[4].bottom = 28;
    ladders[4].top = 84;

    ladders[5].bottom = 40;
    ladders[5].top = 59;

    ladders[6].bottom = 51;
    ladders[6].top = 67;

    ladders[7].bottom = 63;
    ladders[7].top = 81;

    positions[0] = 0;
    positions[1] = 0;
}

void SnakeAndLadder::play(const char* p1, const char* p2, bool vsComputer, int& score1, int& score2) {
    reset();
    score1 = 0;
    score2 = 0;

    set_color(Pink, Black);
    cout << "===== SNAKE AND LADDER =====" << endl;
    set_color(White, Black);
    cout << "First to reach square 100 wins!" << endl;

    set_color(Red, Black);
    cout << "SNAKES  (head -> tail): ";
    set_color(White, Black);
    for (int i = 0; i < NUM_SNAKES; i++) {
        cout << snakes[i].head << "->" << snakes[i].tail;
        if (i < NUM_SNAKES - 1) {
            cout << "  ";
        }
        else {
            cout << endl;
        }
    }

    set_color(Green, Black);
    cout << "LADDERS (bottom -> top): ";
    set_color(White, Black);
    for (int i = 0; i < NUM_LADDERS; i++) {
        cout << ladders[i].bottom << "->" << ladders[i].top;
        if (i < NUM_LADDERS - 1) {
            cout << "  ";
        }
        else {
            cout << endl;
        }
    }
    cout << endl;

    const char* names[2] = { p1, p2 };
    int  currentTurn = 0;
    bool gameOver = false;

    while (!gameOver) {
        set_color(Cyan, Black);
        cout << ">>> " << names[currentTurn] << "'s turn <<<" << endl;
        set_color(White, Black);
        printPositions(p1, p2);

        if (currentTurn == 1 && vsComputer) {
            set_color(Pink, Black);
            cout << "Computer rolls..." << endl;
            set_color(White, Black);
        }
        else {
            set_color(Grey, Black);
            cout << "Press Enter to roll the dice...";
            set_color(White, Black);
            cin.get();
        }

        int dice = rollDice();
        int newPos = positions[currentTurn] + dice;
        set_color(Yellow, Black);
        cout << names[currentTurn] << " rolled a " << dice << endl;
        set_color(White, Black);

        if (newPos > BOARD_SIZE) {
            set_color(Red, Black);
            cout << "Overshot! Need exact roll. Stay at " << positions[currentTurn] << endl;
            set_color(White, Black);
        }
        else {
            positions[currentTurn] = newPos;
            cout << "Moved to square " << newPos << endl;

            positions[currentTurn] = checkLadder(positions[currentTurn]);
            positions[currentTurn] = checkSnake(positions[currentTurn]);

            if (positions[currentTurn] == BOARD_SIZE) {
                printBoard();
                set_color(Yellow, Black);
                cout << "====" << names[currentTurn] << " reached 100 and WINS! ====" << endl;
                set_color(White, Black);
                if (currentTurn == 0) {
                    score1 = 1;
                }
                else {
                    score2 = 1;
                }
                gameOver = true;
                break;
            }
        }

        printBoard();
        currentTurn = 1 - currentTurn;

        if (!vsComputer) {
            pause();
        }
    }

    set_color(Pink, Black);
    cout << "===== GAME OVER =====" << endl;
    set_color(White, Black);
    printPositions(p1, p2);
}