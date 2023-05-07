#pragma once

#include "Header.h"

int offset;
int p_time, g_time, s_time;
int kills;

bool flip, p_Hurt = false;  //flip nhan vat
int deadCount;  //so lan nhan vat chet
bool g_flip=true; //flip goblin
bool g_Dead = false;
bool s_flip=false;  //flip skeleton
bool s_Dead=false;

struct Pair {
    double x, y;
    int xx() { return x; }
    int yy() { return y; }
    Pair(double x = 0, double y = 0) : x(x), y(y) {}
};

Pair p_pos, g_pos, s_pos;
