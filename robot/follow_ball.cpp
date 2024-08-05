#include "background.h"

//move(-100,0) : go forward
//move(0,100) : turn left
//move(0,-100) : turn right
int left = 0;
int right = 0;
int move(int forward, int turn ) 
{
    left = forward + turn;
    right = forward - turn;
    if (left > 1023)
    {
	left = 1023;
    }
    if (left < -1023)
    {
	left = -1023;
    }
    if (right > 1023)
    {
	right = 1023;
    }
    if (right < -1023)
    {
	right = -1023;
    }
    MFSetServoRotaSpd(1,right);
    MFSetServoRotaSpd(2,right);
    MFSetServoRotaSpd(3,-left);
    MFSetServoRotaSpd(4,-left);
    MFServoAction();
}
void follow_the_ball()
{
	char ball_x[100];
	int vx = 0;
	int vy = 0;
	int x = 0;
	int y = 0;
	MFCapSetH(360,0);
	MFCapSetS(255,0);
	MFCapSetI(117,0);
	MFCapOpen();
	DelayMS(500);
	while(1)
	{
		vx = MFCapGetCenterX();
		vy = MFCapGetCenterY();
		x = (vx - 160)*2;
		y = (vy-120)*2;
		if(MFCapGetSum() < 600)
			move(-200,0);
		else
			move(-y,-1*x);
		DelayMS(250);
	}
}
int main(int argc, char* agrv[])
{
	MFInit();
	int switch_ = 1;
	MFSetPortDirect(0x00000000);
	MFDigiInit(20);//初始化IO查询周期为20ms	

	MFSetServoMode(1,1);
	MFSetServoMode(2,1);
	MFSetServoMode(3,1);
	MFSetServoMode(4,1);
	while(1)
	{
		switch_ = MFGetDigiInput(3);
		DelayMS(150);
		if(switch_ == 0)
			break;
	}
	follow_the_ball();
}
