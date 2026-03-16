#include "Match_Cards.h"
#include "Colors.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

MatchCards::MatchCards() {
    srand(time(0));
    scores[0] = 0;
    scores[1] = 0;
    totalMatched = 0;

    for (int i = 0; i < TOTAL_CARDS; i++) {
        cards[i] = (i % (TOTAL_CARDS / 2)) + 1;
        revealed[i] = false;
    }

    shuffleDeck();
}

MatchCards::~MatchCards() {}

void MatchCards::shuffleDeck() {
    for (int i = TOTAL_CARDS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = cards[i];
        cards[i] = cards[j];
        cards[j] = tmp;
    }
}

void MatchCards::printCardValue(int i, int flipA, int flipB) const {
    if (revealed[i]) {
        set_color(Green, Black);
        cout << "[" << cards[i] << "] ";
        set_color(White, Black);
    }
    else if (i == flipA || i == flipB) {
        set_color(Yellow, Black);
        cout << cards[i] << "  ";
        set_color(White, Black);
    }
    else {
        set_color(Cyan, Black);
        int pos = i + 1;
        if (pos < 10) {
            cout << pos << "  ";
        }
        else {
            cout << pos << "  ";
        }
        set_color(White, Black);
    }
}

void MatchCards::printBoard(int flipA, int flipB) const {
    cout << endl;
    set_color(Cyan, Black);
    cout << "     1    2    3    4" << endl;
    cout << "  ====================" << endl;

    for (int r = 0; r < GRID_ROWS; r++) {
        set_color(Cyan, Black);
        cout << " " << (r + 1) << " |";
        for (int c = 0; c < GRID_COLS; c++) {
            int idx = r * GRID_COLS + c;
            printCardValue(idx, flipA, flipB);
            set_color(Cyan, Black);
            cout << "|";
        }
        cout << endl;
        cout << "  =====================" << endl;
    }
    set_color(White, Black);
    cout << endl;
}

bool MatchCards::allMatched() const {
    return (totalMatched == TOTAL_CARDS / 2);
}

int MatchCards::getInput(int low, int high) const {
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

void MatchCards::pause() const {
    set_color(Grey, Black);
    cout << "Press Enter to continue...";
    set_color(White, Black);
    cin.get();
}

void MatchCards::computerPickTwo(int& pick1, int& pick2) const {
    pick1 = -1;
    pick2 = -1;

    for (int i = 0; i < TOTAL_CARDS && pick1 == -1; i++) {
        if (!revealed[i]) {
            pick1 = i;
        }
    }

    for (int i = pick1 + 1; i < TOTAL_CARDS && pick2 == -1; i++) {
        if (!revealed[i]) {
            pick2 = i;
        }
    }
}

void MatchCards::reset() {
    scores[0] = 0;
    scores[1] = 0;
    totalMatched = 0;

    for (int i = 0; i < TOTAL_CARDS; i++) {
        cards[i] = (i % (TOTAL_CARDS / 2)) + 1;
        revealed[i] = false;
    }

    shuffleDeck();
}

void MatchCards::play(const char* p1, const char* p2, bool vsComputer, int& score1, int& score2) {
    reset();

    set_color(Pink, Black);
    cout << "===== MATCH CARDS (MEMORY GAME) =====" << endl;
    set_color(White, Black);
    cout << "Find all 8 matching pairs." << endl;
    cout << "Enter card position (row then column, each 1-4)." << endl;
    cout << "Most pairs wins!" << endl;

    const char* names[2] = { p1, p2 };
    int currentTurn = 0;

    while (!allMatched()) {
        cout << endl;
        set_color(Cyan, Black);
        cout << ">>> " << names[currentTurn] << "'s turn <<<" << endl;
        set_color(White, Black);
        cout << p1 << ": " << scores[0] << " pairs   |   " << p2 << ": " << scores[1] << " pairs" << endl;

        printBoard(-1, -1);

        int i1 = -1;
        int i2 = -1;

        if (currentTurn == 1 && vsComputer) {
            computerPickTwo(i1, i2);
            set_color(Pink, Black);
            cout << "Computer picks card " << (i1 / GRID_COLS + 1) << "," << (i1 % GRID_COLS + 1) << endl;
            set_color(White, Black);
        }
        else {
            int row1, col1;
            set_color(Yellow, Black);
            cout << "Pick first card --- row (1-4): ";
            set_color(White, Black);
            row1 = getInput(1, 4) - 1;
            set_color(Yellow, Black);
            cout << "Pick first card --- col (1-4): ";
            set_color(White, Black);
            col1 = getInput(1, 4) - 1;
            i1 = row1 * GRID_COLS + col1;

            while (revealed[i1]) {
                set_color(Red, Black);
                cout << "Already matched! Pick another." << endl;
                set_color(Yellow, Black);
                cout << "Row (1-4): ";
                set_color(White, Black);
                row1 = getInput(1, 4) - 1;
                set_color(Yellow, Black);
                cout << "Col (1-4): ";
                set_color(White, Black);
                col1 = getInput(1, 4) - 1;
                i1 = row1 * GRID_COLS + col1;
            }
        }

        printBoard(i1, -1);

        if (currentTurn == 1 && vsComputer) {
            set_color(Pink, Black);
            cout << "Computer picks card " << (i2 / GRID_COLS + 1) << ", " << (i2 % GRID_COLS + 1) << endl;
            set_color(White, Black);
        }
        else {
            int row2, col2;
            set_color(Yellow, Black);
            cout << "Pick second card >>> row (1-4): ";
            set_color(White, Black);
            row2 = getInput(1, 4) - 1;
            set_color(Yellow, Black);
            cout << "Pick second card >>> col (1-4): ";
            set_color(White, Black);
            col2 = getInput(1, 4) - 1;
            i2 = row2 * GRID_COLS + col2;

            while (revealed[i2] || i2 == i1) {
                if (i2 == i1) {
                    set_color(Red, Black);
                    cout << "Same card! Pick a different one." << endl;
                    set_color(White, Black);
                }
                else {
                    set_color(Red, Black);
                    cout << "Already matched! Pick another." << endl;
                    set_color(White, Black);
                }
                set_color(Yellow, Black);
                cout << "Row (1-4): ";
                set_color(White, Black);
                row2 = getInput(1, 4) - 1;
                set_color(Yellow, Black);
                cout << "Col (1-4): ";
                set_color(White, Black);
                col2 = getInput(1, 4) - 1;
                i2 = row2 * GRID_COLS + col2;
            }
        }

        printBoard(i1, i2);

        if (cards[i1] == cards[i2]) {
            set_color(Green, Black);
            cout << ">>> MATCH! Both cards show " << cards[i1] << " <<<" << endl;
            set_color(White, Black);
            revealed[i1] = true;
            revealed[i2] = true;
            scores[currentTurn]++;
            totalMatched++;
            cout << names[currentTurn] << " scores a point! Take another turn." << endl;
        }
        else {
            set_color(Red, Black);
            cout << "No match (" << cards[i1] << " vs " << cards[i2] << "). Cards flipped back." << endl;
            set_color(White, Black);
            pause();
            currentTurn = 1 - currentTurn;
        }
    }

    cout << endl;
    set_color(Pink, Black);
    cout << "===== ALL PAIRS FOUND =====" << endl;
    set_color(Yellow, Black);
    cout << p1 << ": " << scores[0] << " pairs" << endl;
    set_color(Green, Black);
    cout << p2 << ": " << scores[1] << " pairs" << endl;
    set_color(White, Black);
    cout << endl;

    if (scores[0] > scores[1]) {
        set_color(Yellow, Black);
        cout << ">>> " << p1 << " WINS! <<<" << endl;
        set_color(White, Black);
    }
    else if (scores[1] > scores[0]) {
        set_color(Green, Black);
        cout << ">>> " << p2 << " WINS! <<<" << endl;
        set_color(White, Black);
    }
    else {
        set_color(Grey, Black);
        cout << ">>> It's a TIE! <<<" << endl;
        set_color(White, Black);
    }

    score1 = scores[0];
    score2 = scores[1];
}