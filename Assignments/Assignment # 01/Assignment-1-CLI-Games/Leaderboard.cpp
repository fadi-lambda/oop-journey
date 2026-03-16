#include "Leaderboard.h"
#include "Colors.h"
#include <iostream>
#include <fstream>
using namespace std;

LeaderboardManager::LeaderboardManager() {
    count = 0;
    strcpy(filename, "leaderboard.txt", FILENAME_LEN);
    loadFromFile();
}

LeaderboardManager::~LeaderboardManager() {
    saveToFile();
}

void LeaderboardManager::strcpy(char* goes, const char* comes, int maxLen) const {
    int i = 0;
    while (i < maxLen - 1 && comes[i] != '\0') {
        goes[i] = comes[i];
        i++;
    }
    goes[i] = '\0';
}

int LeaderboardManager::strlen(const char* s) const {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

bool LeaderboardManager::strcmp(const char* a, const char* b) const {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return false;
        }
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

void LeaderboardManager::sortRecords() {
    for (int i = 1; i < count; i++) {
        Record key = records[i];
        int j = i - 1;
        while (j >= 0 && records[j].score < key.score) {
            records[j + 1] = records[j];
            j--;
        }
        records[j + 1] = key;
    }
}

void LeaderboardManager::addRecord(const char* playerName, const char* gameName, int score) {
    for (int i = 0; i < count; i++) {
        if (strcmp(records[i].playerName, playerName) && strcmp(records[i].gameName, gameName)) {
            if (score > records[i].score) {
                records[i].score = score;
            }
            sortRecords();
            saveToFile();
            return;
        }
    }

    if (count < MAX_RECORDS) {
        strcpy(records[count].playerName, playerName, MAX_NAME_LEN);
        strcpy(records[count].gameName, gameName, MAX_GAME_LEN);
        records[count].score = score;
        count++;
        sortRecords();
        saveToFile();
    }
    else {
        set_color(Red, Black);
        cout << "Leaderboard full! Record not added." << endl;
        set_color(White, Black);
    }
}

void LeaderboardManager::loadFromFile() {
    ifstream fin(filename);
    if (!fin.is_open()) {
        return;
    }

    count = 0;
    while (count < MAX_RECORDS && fin >> records[count].playerName >> records[count].gameName >> records[count].score) {
        count++;
    }

    fin.close();
    sortRecords();
}

void LeaderboardManager::saveToFile() const {
    ofstream fout(filename);
    if (!fout.is_open()) {
        set_color(Red, Black);
        cout << "ERROR: Could not open leaderboard file for writing." << endl;
        set_color(White, Black);
        return;
    }

    for (int i = 0; i < count; i++) {
        fout << records[i].playerName << " " << records[i].gameName << " " << records[i].score << "\n";
    }

    fout.close();
}

void LeaderboardManager::displayLeaderboard() const {
    cout << endl;
    set_color(Yellow, Black);
    cout << "==========================================================" << endl;
    cout << "                      LEADERBOARD                        " << endl;
    cout << "==========================================================" << endl;
    set_color(White, Black);

    if (count == 0) {
        set_color(Grey, Black);
        cout << "No records yet. Play some games!" << endl;
        set_color(Yellow, Black);
        cout << "==========================================================" << endl;
        set_color(White, Black);
        return;
    }

    set_color(Cyan, Black);
    cout << "  Rank  Player              Game                Score" << endl;
    cout << "  ----  ------------------  ------------------  -----" << endl;
    set_color(White, Black);

    for (int i = 0; i < count; i++) {
        int rank = i + 1;
        if (rank == 1) {
            set_color(Pink, Black);
        }
        else if (rank == 2) {
            set_color(Cyan, Black);
        }
        else if (rank == 3) {
            set_color(Green, Black);
        }
        else {
            set_color(White, Black);
        }

        cout << "  ";
        if (rank < 10) {
            cout << "   " << rank;
        }
        else if (rank < 100) {
            cout << "  " << rank;
        }
        else {
            cout << " " << rank;
        }

        cout << "  ";
        int pLen = strlen(records[i].playerName);
        cout << records[i].playerName;
        for (int s = pLen; s < 18; s++) {
            cout << ' ';
        }

        cout << "  ";
        int gLen = strlen(records[i].gameName);
        cout << records[i].gameName;
        for (int s = gLen; s < 18; s++) {
            cout << ' ';
        }

        cout << "  " << records[i].score << endl;
    }

    set_color(Yellow, Black);
    cout << "==========================================================" << endl;
    set_color(White, Black);
}

void LeaderboardManager::displayByGame(const char* gameName) const {
    cout << endl;
    set_color(Yellow, Black);
    cout << "==========================================================" << endl;
    cout << "  Leaderboard --- " << gameName << endl;
    cout << "==========================================================" << endl;
    set_color(White, Black);

    int rank = 0;
    bool found = false;

    for (int i = 0; i < count; i++) {
        if (strcmp(records[i].gameName, gameName)) {
            rank++;
            found = true;

            if (rank == 1) {
                set_color(Pink, Black);
            }
            else if (rank == 2) {
                set_color(Cyan, Black);
            }
            else if (rank == 3) {
                set_color(Green, Black);
            }
            else {
                set_color(White, Black);
            }

            cout << "  ";
            if (rank < 10) {
                cout << "   " << rank;
            }
            else if (rank < 100) {
                cout << "  " << rank;
            }
            else {
                cout << " " << rank;
            }

            cout << "  ";
            int pLen = strlen(records[i].playerName);
            cout << records[i].playerName;
            for (int s = pLen; s < 18; s++) {
                cout << ' ';
            }

            cout << "  " << records[i].score << endl;
        }
    }

    if (!found) {
        set_color(Grey, Black);
        cout << "No records for this game yet." << endl;
    }

    set_color(Yellow, Black);
    cout << "==========================================================" << endl;
    set_color(White, Black);
}

int LeaderboardManager::getCount() const {
    return count;
}