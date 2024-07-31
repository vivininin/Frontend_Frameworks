#pragma once
#include <vector>
#include <string>
#include "servo.h"
#include "basic.h"
#include "wheels.h"
#include "mission.h"
#include "eyes.h"

class Suzaku {
    private:
		Wheels wheels;
		Eyes my_eyes;
		int count;
		int delay_speed;
		bool obs_information[4];
		int forward_speed;
		bool edge_information[6];
        int turn_dir;
    public:
		static const int FRONT = 0;
		static const int LEFT_FRONT= 4;
		static const int RIGHT_FRONT= 2;
		static const int RIGHT= 5;
		static const int LEFT= 1;
		static const int BACK= 3;
		Suzaku();
        void brain();
        void stop();
		void delay_until(int time, int divise_times, bool condition);
		//turn: set speed and dir,
		//dir  = 0, go ahead
		//dir  = 1, go backward
		//dir  = 2, turn right
		//dir  = 3, turn left
		void turn(int speed, int dir);
		void waitforcommand();
		void test_rotate_angle(int speed, int time);

		bool fragment_delay(int delay_time, int divise_num, int dir);
		void Edge_dection();
		void Enemy_dection();
};
