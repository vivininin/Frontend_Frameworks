#pragma once
#include "servo.h"
#include "basic.h"

class Wheels {
	private:
		int direct;//direct: 0->ahead,1->back,2->right,3->left
		int di_speed[4][2];
		int speed;
		Servo servo;
		int servo_num;
	public:
		Wheels();
		void stop();
		void setServoNum(int n);
		void setDirect(int dir);
		void setSpeed(int sp);
		void setMode(int mode);
		void action();
		void init();
};

