#include "Rock_Paper_Scissors.h"
#include "Colors.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

RockPaperScissors::RockPaperScissors() {
    srand(time(0));
    roundsToPlay = 5;
    wins1 = 0;
    wins2 = 0;
    draws = 0;
}

RockPaperScissors::~RockPaperScissors() {}

int RockPaperScissors::getInput(int low, int high) const {
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

int RockPaperScissors::getPlayerChoice(const char* name) const {
    cout << endl;
    set_color(Yellow, Black);
    cout << name << " choose:" << endl;
    set_color(White, Black);
    cout << "1. Rock" << endl;
    cout << "2. Paper" << endl;
    cout << "3. Scissors" << endl;
    cout << "Enter choice: ";
    return getInput(1, 3);
}

int RockPaperScissors::getComputerChoice() const {
    return (rand() % 3) + 1;
}

void RockPaperScissors::printChoice(int choice) const {
    if (choice == 1) {
        cout << "Rock";
    }
    else if (choice == 2) {
        cout << "Paper";
    }
    else {
        cout << "Scissors";
    }
}

int RockPaperScissors::resolveRound(int c1, int c2) const {
    if (c1 == c2) {
        return 0;
    }

    if ((c1 == 1 && c2 == 3) || (c1 == 2 && c2 == 1) || (c1 == 3 && c2 == 2)) {
        return 1;
    }

    return 2;
}

void RockPaperScissors::printRoundResult(int result, const char* p1, const char* p2) const {
    if (result == 1) {
        set_color(Yellow, Black);
        cout << p1 << " wins the round!" << endl;
        set_color(White, Black);
    }
    else if (result == 2) {
        set_color(Green, Black);
        cout << p2 << " wins the round!" << endl;
        set_color(White, Black);
    }
    else {
        set_color(Grey, Black);
        cout << "Draw!" << endl;
        set_color(White, Black);
    }
}

void RockPaperScissors::reset() {
    wins1 = 0;
    wins2 = 0;
    draws = 0;
    roundsToPlay = 5;
}

void RockPaperScissors::play(const char* p1, const char* p2, bool vsComputer, int& score1, int& score2) {
    reset();

    set_color(Pink, Black);
    cout << "===== ROCK PAPER SCISSORS =====" << endl;
    set_color(White, Black);
    cout << "Best of " << roundsToPlay << " rounds" << endl;

    for (int round = 1; round <= roundsToPlay; round++) {
        cout << endl;
        set_color(Cyan, Black);
        cout << "==== Round " << round << " / " << roundsToPlay << " ====" << endl;
        set_color(White, Black);

        int c1 = getPlayerChoice(p1);

        int c2;
        if (vsComputer) {
            c2 = getComputerChoice();
        }
        else {
            c2 = getPlayerChoice(p2);
        }

        cout << endl;
        set_color(Yellow, Black);
        cout << p1 << " chose: ";
        set_color(White, Black);
        printChoice(c1);
        cout << endl;

        set_color(Green, Black);
        cout << p2 << " chose: ";
        set_color(White, Black);
        printChoice(c2);
        cout << endl;

        int result = resolveRound(c1, c2);
        printRoundResult(result, p1, p2);

        if (result == 1) {
            wins1++;
        }
        else if (result == 2) {
            wins2++;
        }
        else {
            draws++;
        }

        set_color(Cyan, Black);
        cout << "Score --- " << p1 << ": " << wins1 << "   " << p2 << ": " << wins2 << "   Draws: " << draws << endl;
        set_color(White, Black);

        int roundsLeft = roundsToPlay - round;
        if (wins1 > wins2 + roundsLeft || wins2 > wins1 + roundsLeft) {
            set_color(Yellow, Black);
            cout << "Result decided early!" << endl;
            set_color(White, Black);
            break;
        }
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
        set_color(Green, Black);
        cout << p2 << " wins the match!" << endl;
        set_color(White, Black);
    }
    else {
        set_color(Grey, Black);
        cout << "Match is a draw!" << endl;
        set_color(White, Black);
    }

    score1 = wins1;
    score2 = wins2;
}