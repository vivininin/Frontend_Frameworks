#include "background.h"

static int io[3] = {0};
int left = 0;
int right = 0;

void print(char temp[100])
{
    MF2X4LCD(temp, 10);
}
//move(100,0) : go back;
//move(-100,0) : go forward
//move(0,100) : turn left
//move(0,-100) : turn right
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
void Adjust_distance()
{
    while(1)
    {
	if(io[1] == 0)
	{
	    break;
	}
	else if(io[0] == 0)
	{
	    move(0,100);
	    DelayMS(100);
	    print("ad_left");
	}
	else if(io[2] == 0)
	{
	    move(0,-100);
	    DelayMS(100);
	    print("ad_right");
	}
    }
}
void Find_Bottle()
{
    bool near_ball = false;
    int vx = 0;
    int vy = 0;
    int lastx = 160;
    MFCapSetH(233,79);
    MFCapSetS(248,85);
    MFCapOpen();
    DelayMS(500);
    while(1)
    {
	for(int i = 0; i < 5; i++)
	{
	    io[i] = MFGetDigiInput(i);
	}
	vx = MFCapGetCenterX();
	vy = MFCapGetCenterY();
	if (!near_ball)
	{
	    if(MFCapGetSum() < 500) //look around
	    {
		if(vx < 100)
		{
		    print("less_sum_left");
		    move(0,300);
		    DelayMS(500);
		}
		else if(vx > 220)
		{
		    print("less_sum_right");
		    move(0,-300);
		    DelayMS(500);
		}
		else
		{
		    print("less_sumgo");
		    move(-300,0);
		    DelayMS(500);
		}
	    }
	    else
		near_ball = true; 
	}
	if(near_ball)
	{
	    if(io[0] == 0 ||io[1] == 0 || io[2] == 0)
	    {
		move(0,0);
		DelayMS(200);
		break;
	    }
	    else
	    {
		print("go");
		vx = (vx - 160)*3;
		move(-300,-vx);
		DelayMS(100);
	    }
	}
    }
    Adjust_distance();
}

int main(int argc, char * argv[])
{
    MFInit();
    int switch_ = 1;
    MFSetPortDirect(0x00000000);
    MFADEnable(0);
    MFADInit(20);	//初始化AD查询周期为20ms
    MFDigiInit(20);//初始化IO查询周期为20ms	

    MFSetServoMode(1,1);
    MFSetServoMode(2,1);
    MFSetServoMode(3,1);
    MFSetServoMode(4,1);
    print("ready_go!");
    while(1)
    {
	switch_ = MFGetDigiInput(3);
	DelayMS(150);
	if(switch_ == 0)
	    break;
    }
    Find_Bottle();

}

