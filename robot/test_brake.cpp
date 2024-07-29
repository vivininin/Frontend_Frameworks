#include "test_brake.h"

const int di_speed[4][2]={1,1,-1,-1,-1,1,1,-1};

void Brake::init()
{
	v0 = 0;
	a = 0;
	end = false;
	reverse_v = 1023;
}

void Brake::turn(int speed, int dir)
{
	for(int i = 1; i<= 2; i++)
		servo.setRotaSpeed(i,di_speed[dir][i-1]*speed);
	servo.action();
}
void Brake::wait_for_command()
{
	while(1)
	{
		if(MFGetDigiInput(3) == 0)
			break;
		DelayMS(50);
	}
}

void Brake::stop()
{
	for(int i=1;i<=2; i++)
		servo.setRotaSpeed(i,0);
	servo.action();
}

void Brake::test_for_turn(int dir)
{
	wait_for_command();
	turn(500,0);
	DelayMS(300);
	turn(500,dir);
	DelayMS(3000);
	stop();
	DelayMS(1000);
}


	
void Brake::step()
{
	init();
	while(1)
	{
		test_for_turn(2);
		test_for_turn(3);
	}
	
}

