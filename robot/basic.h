#pragma once
#include "background.h"
#include "servo.h"
#include "config.h"
#include <algorithm>
void init();
void quit();
void print(const char* s);

inline int sgn(double x) { return (x > 1e-8) - (x < -1e-8); }

inline int cutRange(int x, int xl, int xr){
    return std::max(xl, std::min(xr, x));
}

