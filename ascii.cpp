#include <vector>
#include <iostream>
#include <fstream>
#include <ncurses.h>
#include "ascii.h"

using std::vector;
using std::string;
using std::ifstream;
using std::ios;

using namespace ascii;

int ascii::ascii_mvaddstr(int y, int x, const char* str)
{
    for ( ; x < 0; ++x, ++str)
        if (*str == '\0')  return ERR;
    for ( ; *str != '\0'; ++str, ++x)
        if (mvaddch(y, x, *str) == ERR)  return ERR;
    return OK;
}

int ascii::draw(const Ascii& art, int x, bool FLY) {
    int i, j, y, dy = 0;
    int totallength = art.length;
    if (x < - totallength) return ERR;
    y = LINES / 2 - 3;
    if (FLY) {
        y = (x / 6) + LINES - (COLS / 6) - art.height;
        dy = 2; 
    }
    int frame = (art.length+x) % art.numframes;
    for (i = 0; i < art.frames[frame].size(); ++i) {
        ascii_mvaddstr(y + i, x, art.frames[frame][i].c_str());
    }
    return OK;
}

Ascii ascii::createascii(string filename) {
    ifstream file;
    int i = 0, j = 0, height, length, numframes;
    file.open(filename, ios::in);
    if (!file.is_open()) return Ascii();
    // Get metadata
    file >> height;
    file >> length;
    file >> numframes;
    // Create struct with metadata values
    Ascii out = { height, length + 1, numframes, vector<vector<string>>(numframes, {height+1,""}) }; 
    string temp;
    getline(file, temp); // Ignore first metadata line
    for (i = 0; i < numframes; ++i) {
        for (j = 0; j < height; ++j) {
            if (!getline(file, temp)) break;
            out.frames[i][j] = " " + temp + string(out.length - temp.size(), ' '); // Add spaces on ends to "erase" movement trails
        }
        out.frames[i].back() = string(length, ' '); // Add empty line at bottom to "erase" movement trails
    }
    file.close();
    return out;
}
