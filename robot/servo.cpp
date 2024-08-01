#include "servo.h"
#include "basic.h"

void Servo::setMode(int id, int mode){
    MFSetServoMode(id, mode);
}
void Servo::setPos(int id, int pos, int speed){
    //TODO: add assert(getMode(id) == MODE_ANGLE)
    pos = cutRange(pos, 0, 1023);
    speed = cutRange(speed, 0, 1023);
    MFSetServoPos(id, pos, speed);
}
void Servo::setRotaSpeed(int id, int speed){
    //TODO: add assert(getMode(id) == MODE_MOTOR)
    speed = cutRange(speed, -1023, 1023);
    MFSetServoRotaSpd(id, speed);
}
int Servo::getPos(int id){
    //TODO: add assert(getMode(id) == MODE_ANGLE)
    return MFGetServoPos(id);
}
void Servo::action(){
    MFServoAction();
}
