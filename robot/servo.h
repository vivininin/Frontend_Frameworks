#pragma once
#include "background.h"
class Servo{
    public:
        static const int MODE_ANGLE = 0;
        static const int MODE_MOTOR = 1;
        static void setMode(int id, int mode);
        static void setPos(int id, int pos, int speed);
        static void setRotaSpeed(int id, int speed);
        static int getPos(int id);
        static void action();
};
