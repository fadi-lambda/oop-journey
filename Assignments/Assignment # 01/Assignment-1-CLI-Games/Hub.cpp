#include "Hub.h"
#include "Colors.h"
#include <iostream>
using namespace std;

Hub::Hub() {
    running = true;
}

Hub::~Hub() {}

void Hub::clearScreen() const {
    system("cls");
}

void Hub::printBanner() const {
    set_color(Pink, Black);
    cout << "==========================================================" << endl;
    cout << "         WELCOME TO THE PERSONAL GAMING HUB              " << endl;
    cout << "         Muhammad Fahad Basharat | 25I-3081              " << endl;
    cout << "==========================================================" << endl;
    set_color(White, Black);
    cout << endl;
}

void Hub::printMainMenu() const {
    set_color(Cyan, Black);
    cout << "MAIN MENU" << endl;
    cout << "---------" << endl;
    set_color(Yellow, Black);
    cout << "1. Tic Tac Toe" << endl;
    cout << "2. Snake and Ladder" << endl;
    cout << "3. Rock Paper Scissors" << endl;
    cout << "4. Match Cards (Memory Game)" << endl;
    cout << "5. Number Guessing Game" << endl;
    set_color(Green, Black);
    cout << "6. Leaderboard" << endl;
    set_color(Red, Black);
    cout << "7. Exit" << endl;
    set_color(White, Black);
    cout << endl;
    cout << "Enter choice: ";
}

void Hub::printModeMenu() const {
    set_color(Cyan, Black);
    cout << endl;
    cout << "SELECT MODE" << endl;
    cout << "-----------" << endl;
    set_color(Yellow, Black);
    cout << "1. Player vs Player" << endl;
    cout << "2. Player vs Computer" << endl;
    set_color(Red, Black);
    cout << "3. Back to Main Menu" << endl;
    set_color(White, Black);
    cout << endl;
    cout << "Enter choice: ";
}

void Hub::pause() const {
    set_color(Grey, Black);
    cout << endl;
    cout << "Press Enter to continue...";
    set_color(White, Black);
    cin.ignore();
}

int Hub::getInput(int low, int high) const {
    int val;
    while (true) {
        if (cin >> val) {
            cin.ignore();
            if (val >= low && val <= high) {
                return val;
            }
            set_color(Red, Black);
            cout << "Invalid! Enter a number between " << low << " and " << high << ": ";
            set_color(White, Black);
        }
        else {
            cin.clear();
            char ch;
            while (cin.get(ch) && ch != '\n');
            set_color(Red, Black);
            cout << "Invalid! Enter a number between " << low << " and " << high << ": ";
            set_color(White, Black);
        }
    }
}

void Hub::readName(char* playerName, int maxLen) const {
    int i = 0;
    char c = cin.get();
    while (c != '\n' && i < maxLen - 1) {
        playerName[i++] = c;
        c = cin.get();
    }
    playerName[i] = '\0';
}

void Hub::launchGame(int gameChoice) {
    clearScreen();
    printBanner();
    printModeMenu();

    int mode = getInput(1, 3);
    if (mode == 3) {
        return;
    }

    bool vsComputer;
    if (mode == 2) {
        vsComputer = true;
    }
    else {
        vsComputer = false;
    }

    char p1[MAX_NAME_LEN];
    char p2[MAX_NAME_LEN];

    cout << endl;
    set_color(Cyan, Black);
    cout << "Enter Player 1 name: ";
    set_color(White, Black);
    readName(p1, MAX_NAME_LEN);

    if (vsComputer) {
        const char* cName = "Computer";
        int i = 0;
        while (cName[i] != '\0' && i < MAX_NAME_LEN - 1) {
            p2[i] = cName[i];
            i++;
        }
        p2[i] = '\0';
    }
    else {
        set_color(Cyan, Black);
        cout << "Enter Player 2 name: ";
        set_color(White, Black);
        readName(p2, MAX_NAME_LEN);
    }

    clearScreen();
    printBanner();

    int score1 = 0;
    int score2 = 0;

    if (gameChoice == 1) {
        ticTacToe.reset();
        ticTacToe.play(p1, p2, vsComputer, score1, score2);
        leaderboard.addRecord(p1, "TicTacToe", score1);
        leaderboard.addRecord(p2, "TicTacToe", score2);
    }
    else if (gameChoice == 2) {
        snakeAndLadder.reset();
        snakeAndLadder.play(p1, p2, vsComputer, score1, score2);
        leaderboard.addRecord(p1, "Snake_N_Ladder", score1);
        leaderboard.addRecord(p2, "Snake_N_Ladder", score2);
    }
    else if (gameChoice == 3) {
        rockPaperScissors.reset();
        rockPaperScissors.play(p1, p2, vsComputer, score1, score2);
        leaderboard.addRecord(p1, "RockPaperScissor", score1);
        leaderboard.addRecord(p2, "RockPaperScissor", score2);
    }
    else if (gameChoice == 4) {
        matchCards.reset();
        matchCards.play(p1, p2, vsComputer, score1, score2);
        leaderboard.addRecord(p1, "MatchCards", score1);
        leaderboard.addRecord(p2, "MatchCards", score2);
    }
    else if (gameChoice == 5) {
        numberGuessingGame.reset();
        numberGuessingGame.play(p1, p2, vsComputer, score1, score2);
        leaderboard.addRecord(p1, "NumberGuessing", score1);
        leaderboard.addRecord(p2, "NumberGuessing", score2);
    }

    set_color(Green, Black);
    cout << endl;
    cout << "Results saved to leaderboard!" << endl;
    set_color(White, Black);
    pause();
}

void Hub::showLeaderboardMenu() const {
    clearScreen();
    printBanner();

    set_color(Cyan, Black);
    cout << "LEADERBOARD MENU" << endl;
    cout << "----------------" << endl;
    set_color(Yellow, Black);
    cout << "1. View Full Leaderboard" << endl;
    cout << "2. View by Game" << endl;
    set_color(Red, Black);
    cout << "3. Back" << endl;
    set_color(White, Black);
    cout << endl;
    cout << "Enter choice: ";

    int choice = getInput(1, 3);

    if (choice == 1) {
        leaderboard.displayLeaderboard();
        pause();
    }
    else if (choice == 2) {
        set_color(Cyan, Black);
        cout << endl;
        cout << "SELECT GAME" << endl;
        cout << "-----------" << endl;
        set_color(Yellow, Black);
        cout << "1. Tic Tac Toe" << endl;
        cout << "2. Snake and Ladder" << endl;
        cout << "3. Rock Paper Scissors" << endl;
        cout << "4. Match Cards" << endl;
        cout << "5. Number Guessing Game" << endl;
        set_color(White, Black);
        cout << endl;
        cout << "Enter choice: ";

        int g = getInput(1, 5);

        if (g == 1) {
            leaderboard.displayByGame("TicTacToe");
        }
        else if (g == 2) {
            leaderboard.displayByGame("Snake_N_Ladder");
        }
        else if (g == 3) {
            leaderboard.displayByGame("RockPaperScissor");
        }
        else if (g == 4) {
            leaderboard.displayByGame("MatchCards");
        }
        else if (g == 5) {
            leaderboard.displayByGame("NumberGuessing");
        }

        pause();
    }
}

void Hub::run() {
    while (running) {
        clearScreen();
        printBanner();
        printMainMenu();

        int choice = getInput(1, 7);

        if (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5) {
            launchGame(choice);
        }
        else if (choice == 6) {
            showLeaderboardMenu();
        }
        else if (choice == 7) {
            running = false;
            clearScreen();
            set_color(Pink, Black);
            cout << "Thank you for playing! Goodbye." << endl;
            set_color(White, Black);
        }
    }
}