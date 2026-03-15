#include "LeaderboardManager.h"

using namespace std;

LeaderboardManager::LeaderboardManager(const char* file) {
    int i = 0;
    while (file[i] != '\0' && i < 99) {
        filename[i] = file[i];
        i++;
    }
    filename[i] = '\0';
    records = nullptr;
    recordCount = 0;
    capacity = 0;
    loadFromFile();
}

LeaderboardManager::~LeaderboardManager() {
    saveToFile();
    if (records) {
        delete[] records;
        records = nullptr;
    }
}

void LeaderboardManager::grow() {
    int newCapacity = (capacity == 0) ? 8 : capacity * 2;
    LeaderboardEntry* newArr = new LeaderboardEntry[newCapacity];
    for (int i = 0; i < recordCount; i++) {
        newArr[i] = records[i];
    }
    if (records) delete[] records;
    records = newArr;
    capacity = newCapacity;
}

void LeaderboardManager::sortRecords() {
    // Simple bubble sort by score descending
    for (int i = 0; i < recordCount - 1; i++) {
        for (int j = 0; j < recordCount - i - 1; j++) {
            if (records[j].score < records[j + 1].score) {
                LeaderboardEntry temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
}

int LeaderboardManager::textLength(const char* text) const {
    int len = 0;
    while (text[len] != '\0') len++;
    return len;
}

void LeaderboardManager::textCopy(char* dst, const char* src, int maxLen) const {
    int i = 0;
    while (src[i] != '\0' && i < maxLen - 1) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

bool LeaderboardManager::textEqual(const char* a, const char* b) const {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

void LeaderboardManager::loadFromFile() {
    if (records) {
        delete[] records;
        records = nullptr;
    }
    recordCount = 0;
    capacity = 0;

    ifstream fin(filename);
    if (!fin.is_open()) {
        return; // File doesn't exist yet — start fresh
    }

    char pName[50], gName[50];
    int sc;
    while (fin >> pName >> gName >> sc) {
        if (recordCount >= capacity) grow();
        textCopy(records[recordCount].playerName, pName, 50);
        textCopy(records[recordCount].gameName, gName, 50);
        records[recordCount].score = sc;
        recordCount++;
    }
    fin.close();
    sortRecords();
}

void LeaderboardManager::saveToFile() {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cerr << "Error: Could not open leaderboard file for writing." << endl;
        return;
    }
    for (int i = 0; i < recordCount; i++) {
        fout << records[i].playerName << " "
            << records[i].gameName << " "
            << records[i].score << "\n";
    }
    fout.close();
}

void LeaderboardManager::addRecord(const char* playerName, const char* gameName, int score) {
    // Check if player already has an entry for this game — update if higher
    bool found = false;
    for (int i = 0; i < recordCount; i++) {
        if (textEqual(records[i].playerName, playerName) && textEqual(records[i].gameName, gameName)) {
            if (score > records[i].score) {
                records[i].score = score;
            }
            found = true;
            break;
        }
    }
    if (!found) {
        if (recordCount >= capacity) grow();
        textCopy(records[recordCount].playerName, playerName, 50);
        textCopy(records[recordCount].gameName, gameName, 50);
        records[recordCount].score = score;
        recordCount++;
    }
    sortRecords();
    saveToFile();
}

LeaderboardEntry* LeaderboardManager::getRecordsByGame(const char* gameName, int& outCount) const {
    // First pass: count matches
    outCount = 0;
    for (int i = 0; i < recordCount; i++) {
        if (textEqual(records[i].gameName, gameName)) {
            outCount++;
        }
    }
    if (outCount == 0) return nullptr;

    LeaderboardEntry* result = new LeaderboardEntry[outCount];
    int idx = 0;
    for (int i = 0; i < recordCount; i++) {
        if (textEqual(records[i].gameName, gameName)) {
            result[idx] = records[i];
            idx++;
        }
    }
    return result;
}

int LeaderboardManager::getRecordCount() const {
    return recordCount;
}

const LeaderboardEntry& LeaderboardManager::getRecord(int index) const {
    return records[index];
}