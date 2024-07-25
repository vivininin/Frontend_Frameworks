#include "wheels.h"

const int di[4][2]={1,1,-1,-1,-1,1,1,-1};



Wheels::Wheels()
{
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
		//dir = 0; go ahead
		//dir = 1; go backward
		//dir = 2; turn right
		//dir = 3; turn left
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

