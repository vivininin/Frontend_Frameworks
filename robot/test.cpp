#include "test.h"
#include "servo.h"
#include "config.h"
#include "basic.h"
#include <cstdio>

static Config & C = Config::getInstance();

const int N = 2;
const double XS[N] = {-0.105, 0.105}; // in m
const double HS[N] = {0, 0}; // in m
const double IDS[N] = {3, 1}; // in m
const double DIRS[N] = {1, -1}; // in m

void moveTurn(double vf, double w){
    double SpeedPerMeter = C.getConfig("SpeedPerMeter");
    double vx, vy, v;
    int i;
    for(i = 0; i < N; i++){
        vx = HS[i] * w;
        vy = vf - XS[i] * w;
        if(sgn(vy) == 0) v = 0;
        else v = vx * vx / vy + vy;
        printf("v[%d] = %.4lf(%d)\n", i, v, (int)(v * SpeedPerMeter));
        Servo::setRotaSpeed(IDS[i], DIRS[i] * v * SpeedPerMeter);
    }
    Servo::action();
}

bool isPush(){
    return MFGetDigiInput(0) == 0;
}

void test1(){
    // servo motor mode test
    C.initConfig("v1", 100);
    C.initConfig("v2", -200);
    MFSetPortDirect(0xff8);
    Servo::setMode(1, Servo::MODE_MOTOR);
    Servo::setMode(3, Servo::MODE_MOTOR);
    int i;
    for(i = 0; i < 15;i++){
        Servo::setRotaSpeed(1, C.getConfig("v1"));
        Servo::setRotaSpeed(3, C.getConfig("v2"));
        Servo::action();
        char buf[15];
        sprintf(buf, "s=%d\n", i);
        print(buf);
        if(isPush()){
            print("interrupt\n");
            break;
        }
        DelayMS(1000);
    }
}

void test2(){
    // servo angle mode test
    C.initConfig("angle1", 100);
    MFSetPortDirect(0xff8);
    Servo::setMode(3, Servo::MODE_ANGLE);
    Servo::action();
    while(1){
        DelayMS(500);
        int pos = Servo::getPos(3);
        char buf[15];
        sprintf(buf, "p=%d\n", pos);
        print(buf);
        Servo::setPos(3, C.getConfig("angle1"), 400);
        Servo::action();
        if(isPush()){
            print("interrupt\n");
            break;
        }
    }
}

void test3(){
    // sensor's test
    MFSetPortDirect(0xff8);
    int left, right;
    char buf[15];
    while(1){
        left = MFGetDigiInput(1);
        right = MFGetDigiInput(2);
        sprintf(buf, "l:%d,r:%d\n", left, right);
        print(buf);
        if(isPush()){
            print("interrupt\n");
            break;
        }
        DelayMS(200);
    }
}

void test4(){
    const double PI = 3.1415926;
    // dodge test
    MFSetPortDirect(0xff8);
    C.initConfig("t4_maxv", 0.4);
    C.initConfig("t4_maxw", PI);
    C.initConfig("SpeedPerMeter", 1024.0);
    Servo::setMode(1, Servo::MODE_MOTOR);
    Servo::setMode(3, Servo::MODE_MOTOR);
    bool left, right;
    double maxv = C.getConfig("t4_maxv");
    double maxw = C.getConfig("t4_maxw");
    char buf[20];
    while(1){
        left = (MFGetDigiInput(1) == 0);
        right = (MFGetDigiInput(2) == 0);
        sprintf(buf, "l:%d,r:%d\n", left, right);
        print(buf);
        if(left and right){
            // turn back
            moveTurn(0, PI);
            print("turn back");
            DelayMS(1000);
        }else if(left){
            // turn right
            moveTurn(maxv/2, maxw);
            print("turn right");
            DelayMS(500);
        }else if(right){
            // turn left
            moveTurn(maxv/2, -maxw);
            print("turn left");
            DelayMS(500);
        }else{
            // move forward
            moveTurn(maxv, 0);
        }
        if(isPush()){
            print("interrupt\n");
            break;
        }
        DelayMS(150);
    }
    quit();
    DelayMS(3000);
}

void test5(){
    const double PI = 3.1415926;
    MFSetPortDirect(0xff8);
    moveTurn(0.2, PI/2);
    while(1){
        if(isPush()){
            print("interrupt\n");
            break;
        }
        DelayMS(500);
    }
    quit();
    DelayMS(2500);
}
