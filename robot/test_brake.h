#pragma once
#include <string>
#include <vector>
#include "servo.h"
#include "mission.h"
#include "basic.h"

class Brake {
	private:
		int v0;
		int a;
		bool end;
		int reverse_v;
		Servo servo;
	public:
		void init();
		void stop();
		void step();
		void turn(int speed, int dir);
		void isEnd();
		void test_for_turn(int dir);
		void wait_for_command();
};
