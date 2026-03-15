#pragma once

#include <fstream>
#include <iostream>

using namespace std;

struct LeaderboardEntry {
    char playerName[50];
    char gameName[50];
    int score;
};

class LeaderboardManager {
private:
    char filename[100];
    LeaderboardEntry* records;
    int recordCount;
    int capacity;

    void grow();
    void sortRecords();
    int textLength(const char* text) const;
    void textCopy(char* dst, const char* src, int maxLen) const;
    bool textEqual(const char* a, const char* b) const;

public:
    LeaderboardManager(const char* file = "snake_leaderboard.txt");
    ~LeaderboardManager();

    void loadFromFile();
    void saveToFile();
    void addRecord(const char* playerName, const char* gameName, int score);

    // Returns a new[] allocated array — caller must delete[]
    // outCount is set to the number of entries returned
    LeaderboardEntry* getRecordsByGame(const char* gameName, int& outCount) const;

    int getRecordCount() const;
    const LeaderboardEntry& getRecord(int index) const;
};