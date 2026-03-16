#pragma once
#include <windows.h>

const int Black = 0;
const int Blue = 1;
const int Green = 2;
const int Cyan = 3;
const int Red = 4;
const int Magenta = 5;
const int Yellow = 6;
const int White = 7;
const int Grey = 8;
const int Pink = 12;

inline void set_color(int text, int background) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (background << 4) | text);
}