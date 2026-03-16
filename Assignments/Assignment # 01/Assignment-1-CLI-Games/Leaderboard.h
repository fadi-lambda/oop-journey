#pragma once

const int MAX_RECORDS = 200;
const int MAX_NAME_LEN = 32;
const int MAX_GAME_LEN = 32;
const int FILENAME_LEN = 64;

struct Record {
    char playerName[MAX_NAME_LEN];
    char gameName[MAX_GAME_LEN];
    int score;
};

class LeaderboardManager {
private:
    Record records[MAX_RECORDS];
    int count;
    char filename[FILENAME_LEN];

    void strcpy(char* goes, const char* comes, int maxLen) const;
    int  strlen(const char* s) const;
    bool strcmp(const char* a, const char* b) const;
    void sortRecords();

public:
    LeaderboardManager();
    ~LeaderboardManager();

    void addRecord(const char* playerName, const char* gameName, int score);
    void loadFromFile();
    void saveToFile() const;
    void displayLeaderboard() const;
    void displayByGame(const char* gameName) const;
    int  getCount() const;
};