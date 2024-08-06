#pragma once
#include "servo.h"

class Flag{
	private:
		Flag(){}
		static const int ID = 3;
		static const int MID = 512;
	public:
		static void init(){
			Servo::setMode(ID, Servo::MODE_ANGLE);
		}
		static void setAngle(int a){
			Servo::setPos(ID, MID + a * 1024/300, 800);
		}
};
