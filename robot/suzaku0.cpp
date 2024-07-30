#include "suzaku.h"

const int di[4][2]={1,1,-1,-1,-1,1,1,-1};

Wheels::Wheels()
{
	direct=0;
	speed= 0;
	servo_num=2;
	for(int i=0;i<4;i++)
		for(int j=0;j<2;j++)
			di_speed[i][j]=di[i][j];
}
void Wheels::stop()
{
	for(int i=1;i<=servo_num;i++)
		servo.setRotaSpeed(i,0);
	servo.action();
}
void Wheels::setServoNum(int n){
	servo_num=n;
}
void Wheels::setDirect(int dir){
	for(int i=1;i<=2;i++)
		servo.setRotaSpeed(i,di_speed[dir][i-1]*speed);
}
void Wheels::setSpeed(int sp){
	speed=sp;
}
void Wheels::setMode(int mode){
	for(int i=1;i<=servo_num;i++)
		servo.setMode(i,mode);
}
void Wheels::action(){
	servo.action();
}	
void Wheels::init(){
	setMode(1);
	setDirect(direct);
}


//This function return a binary number, eg: 000 means there is nothing;
//111means there is Obstacle in the front,left,right side; so as it
bool Eyes::Get_Obs_information(bool information[]){
	information=0;
	bool frontObs = (MFGetDigiInput(2) ==0);
	bool leftObs = (MFGetDigiInput(4) ==0);
	bool rightObs = (MFGetDigiInput(3) ==0); //information = (rightObs<<2)+(leftObs<<1) + frontObs;
	if(information != 0)
	{
		IsObs = true;
	}
	printf("Information:%d ", information);
	return IsObs;
}
bool Eyes::Get_Edge_information(bool information[])
{
	for(int i = 0; i < 4; i++)
		information[i] = false;
	//information[0] == true It need to go backward
	//information[1] == true It need to go forward
	//information[2] == true It need to turn rigth 
	//information[3] == true It need to turn left
	if(MFGetDigiInput(UPFL) == 1 || MFGetDigiInput(UPFR) == 1)
		information[0] = true;
	
	if(MFGetDigiInput(UPBL) == 1 || MFGetDigiInput(UPBR) == 1)
		information[1] = true;
	
	if(MFGetDigiInput(UPFR) == 1 || MFGetDigiInput(UPBR) == 1)
		information[3] = true;

	if(MFGetDigiInput(UPFL) == 1 || MFGetDigiInput(UPBL) == 1)
		information[2] = true;
	for(int i = 0; i < 4; i++)
	{
		if(information[i] == true)
		{
			return true;
		}
	}
	return false;
}

void Suzaku::stop()
{
	wheels.stop();
}

bool Suzaku::fragment_delay(int delay_time, int divise_num, int dir)
{
	int dt = 0;
	int divise_time = delay_time/divise_num;
	for(int i = 0; i < divise_num; i++)
	{
		DelayMS(divise_time);	
		my_eyes.Get_Edge_information(edge_information);
		if(edge_information[dir] == true)
			return true;
	}
	return false;
}

void Suzaku::delay_until(int time, int condition)
{
	while(1)
	{
		DelayMS(time);
		my_eyes.Get_Edge_information(edge_information);
		if(edge_information[condition] == false)
			break;
		time -=200;
		if(time <= 50)
			time = 50;
	}
	return;
}

void Suzaku::Edge_dection()
{	
	if(my_eyes.Get_Edge_information(edge_information) == true)
	{
		bool front_edge = edge_information[0];
		bool back_edge = edge_information[1];
		bool left_edge = edge_information[2];
		bool right_edge = edge_information[3];
		int move_backward = 1; // move back by default
		if(back_edge == true && front_edge == true)
		{
			if(left_edge == true)
				turn(400,2);
			else 
				turn(400,3);
			DelayMS(300);
		}
		else
		{
			if(back_edge == true)
			{
				move_backward = 0;
			}
			turn(800,move_backward);
			DelayMS(400);
			// stop point for test
			stop(); DelayMS(1200);
			//delay_until(400,1 - move_backward);
			/*(if(fragment_delay(100, 4, move_backward))
			  {
			  if(left_edge == true)
			  turn(400,2);
			  else 
			  turn(400,3);
			  DelayMS(300);
			  }*/
			turn_dir = 3;//turn left by default
			if(left_edge == true)
				turn_dir= 2;
		}
	}
	else
	{
		if(turn_dir != -1){
			turn(500,turn_dir);
			DelayMS(900);
		}
		turn(350,0);
		DelayMS(10);
	}
}

void Suzaku::brain()
{
	while(1)
	{
		Edge_dection();
	}
}

