#include "Number_Guessing_Game.h"
#include "Colors.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

NumberGuessingGame::NumberGuessingGame() {
    srand(time(0));
    secretNumber = 0;
    attemptsLeft = MAX_ATTEMPTS;
    wins[0] = 0;
    wins[1] = 0;
}

NumberGuessingGame::~NumberGuessingGame() {}

int NumberGuessingGame::generateSecretNumber() const {
    return rand() % 100 + 1;
}

int NumberGuessingGame::getInput(int low, int high) const {
    int val;
    while (true) {
        cin >> val;
        if (val < low || val > high) {
            cin.clear();
            cin.ignore();
            set_color(Red, Black);
            cout << "Invalid. Enter a number between " << low << " and " << high << ": ";
            set_color(White, Black);
        }
        else {
            cin.ignore();
            return val;
        }
    }
}

void NumberGuessingGame::printAttempt() const {
    set_color(Cyan, Black);
    cout << "Attempts left [";
    for (int i = 0; i < MAX_ATTEMPTS; i++) {
        if (i < attemptsLeft) {
            set_color(Green, Black);
            cout << '|';
        }
        else {
            set_color(Red, Black);
            cout << '.';
        }
    }
    set_color(Cyan, Black);
    cout << "] " << attemptsLeft << endl;
    set_color(White, Black);
}

int NumberGuessingGame::calcScore(int attemptsUsed) const {
    if (attemptsUsed > MAX_ATTEMPTS) {
        return 0;
    }
    return (MAX_ATTEMPTS - attemptsUsed + 1);
}

int NumberGuessingGame::playerRound(const char* name, bool isComputer) {
    attemptsLeft = MAX_ATTEMPTS;
    secretNumber = generateSecretNumber();
    int attemptsUsed = 0;

    int compLow = MIN;
    int compHigh = MAX;

    cout << endl;
    set_color(Cyan, Black);
    cout << name << ", guess the number between " << MIN << " and " << MAX << "!" << endl;
    cout << "You have " << MAX_ATTEMPTS << " attempts." << endl;
    set_color(White, Black);
    cout << endl;

    while (attemptsLeft > 0) {
        printAttempt();

        int guess = 0;

        if (isComputer) {
            guess = (compLow + compHigh) / 2;
            set_color(Pink, Black);
            cout << "Computer guesses: " << guess << endl;
            set_color(White, Black);
        }
        else {
            set_color(Yellow, Black);
            cout << name << ", enter guess: ";
            set_color(White, Black);
            guess = getInput(MIN, MAX);
        }

        attemptsLeft--;
        attemptsUsed++;

        if (guess == secretNumber) {
            set_color(Green, Black);
            cout << "Correct! The number was " << secretNumber << "." << endl;
            cout << "Guessed in " << attemptsUsed << " attempt(s)!" << endl;
            set_color(White, Black);
            return attemptsUsed;
        }
        else if (guess < secretNumber) {
            set_color(Yellow, Black);
            cout << "Too LOW!";
            set_color(White, Black);
            if (isComputer) {
                compLow = guess + 1;
            }
        }
        else {
            set_color(Red, Black);
            cout << "Too HIGH!";
            set_color(White, Black);
            if (isComputer) {
                compHigh = guess - 1;
            }
        }

        int diff = secretNumber - guess;
        if (diff < 0) {
            diff = -diff;
        }

        if (diff <= 5) {
            set_color(Red, Black);
            cout << "(Very hot!)" << endl;
            set_color(White, Black);
        }
        else if (diff <= 15) {
            set_color(Yellow, Black);
            cout << "(Hot)" << endl;
            set_color(White, Black);
        }
        else if (diff <= 30) {
            set_color(Green, Black);
            cout << "(Warm)" << endl;
            set_color(White, Black);
        }
        else {
            set_color(Cyan, Black);
            cout << "(Cold)" << endl;
            set_color(White, Black);
        }
    }

    set_color(Red, Black);
    cout << "Out of attempts! The number was " << secretNumber << "." << endl;
    set_color(White, Black);
    return MAX_ATTEMPTS + 1;
}

void NumberGuessingGame::reset() {
    wins[0] = 0;
    wins[1] = 0;
    attemptsLeft = MAX_ATTEMPTS;
    secretNumber = 0;
}

void NumberGuessingGame::play(const char* p1, const char* p2, bool vsComputer, int& score1, int& score2) {
    reset();
    score1 = 0;
    score2 = 0;

    set_color(Pink, Black);
    cout << "===== NUMBER GUESSING GAME =====" << endl;
    set_color(White, Black);
    cout << "Range : " << MIN << " - " << MAX << endl;
    cout << "Rounds: " << ROUNDS << endl;
    cout << "Max attempts per round: " << MAX_ATTEMPTS << endl;
    set_color(Yellow, Black);
    cout << "Fewer attempts = higher score!" << endl;
    set_color(White, Black);

    int totalScore1 = 0;
    int totalScore2 = 0;

    for (int round = 1; round <= ROUNDS; round++) {
        cout << endl;
        set_color(Cyan, Black);
        cout << "======== ROUND " << round << " / " << ROUNDS << " ========" << endl;
        set_color(White, Black);

        int used1 = playerRound(p1, false);
        int pts1 = calcScore(used1);
        totalScore1 += pts1;
        set_color(Yellow, Black);
        cout << p1 << " earns " << pts1 << " point(s) this round." << endl;
        set_color(White, Black);

        cout << endl;
        cout << "  ----------------------------------------" << endl;

        int used2 = playerRound(p2, vsComputer);
        int pts2 = calcScore(used2);
        totalScore2 += pts2;
        set_color(Green, Black);
        cout << p2 << " earns " << pts2 << " point(s) this round." << endl;
        set_color(White, Black);

        cout << endl;
        set_color(Cyan, Black);
        cout << "Round " << round << " summary:" << endl;
        cout << p1 << ": " << pts1 << " pts   " << p2 << ": " << pts2 << " pts" << endl;
        set_color(White, Black);

        if (pts1 > pts2) {
            wins[0]++;
            set_color(Yellow, Black);
            cout << p1 << " wins round " << round << "!" << endl;
            set_color(White, Black);
        }
        else if (pts2 > pts1) {
            wins[1]++;
            set_color(Green, Black);
            cout << p2 << " wins round " << round << "!" << endl;
            set_color(White, Black);
        }
        else {
            set_color(Grey, Black);
            cout << "Round " << round << " is a draw!" << endl;
            set_color(White, Black);
        }

        set_color(Cyan, Black);
        cout << "Running total --- " << p1 << ": " << totalScore1 << "   " << p2 << ": " << totalScore2 << endl;
        set_color(White, Black);
    }

    cout << endl;
    set_color(Pink, Black);
    cout << "===== FINAL RESULT =====" << endl;
    set_color(Yellow, Black);
    cout << p1 << " total score: " << totalScore1 << endl;
    set_color(Green, Black);
    cout << p2 << " total score: " << totalScore2 << endl;
    set_color(White, Black);
    cout << endl;

    if (totalScore1 > totalScore2) {
        set_color(Yellow, Black);
        cout << "=====" << p1 << " WINS THE MATCH! =====" << endl;
        set_color(White, Black);
    }
    else if (totalScore2 > totalScore1) {
        set_color(Green, Black);
        cout << "=====" << p2 << " WINS THE MATCH! ====" << endl;
        set_color(White, Black);
    }
    else {
        set_color(Grey, Black);
        cout << "===== MATCH IS A DRAW! =====" << endl;
        set_color(White, Black);
    }

    score1 = totalScore1;
    score2 = totalScore2;
}