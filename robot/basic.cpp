#include "basic.h"
#include "servo.h"
#include "config.h"
#include <cstring>
#include <cstdlib>

static Config & C = Config::getInstance();

void init(){
    MFInit();
    MFServoInit(100);
    MFDigiInit(100);
}
void quit(){
    //Servo::action();
	;
}
void print(const char* s){
    MF2X4LCD(s, strlen(s));
#ifndef FAKE
    printf(s);
#endif
}

