#include "suzaku.h"
//turn: set speed and dir,
//dir  = 0, go ahead
//dir  = 1, go backward
//dir  = 2, turn right
//dir  = 3, turn left

Suzaku::Suzaku()
{
	count = 1;
	forward_speed = 400;
	delay_speed = 1;
	for(int i = 0; i < 4; i++)
		obs_information[i] = false;
	for(int i = 0; i < 8; i++)
		edge_information[i] = false;

}
void Suzaku::turn(int speed,int dir)
{
	wheels.setSpeed(speed);
	wheels.setDirect(dir);
	wheels.action();
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

void Suzaku::delay_until(int time, int divise_times,bool condition)
{
	return;
}


void Suzaku::Edge_dection()
{	
	forward_speed = 400;
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
				move_backward = 0;

			//printf("Edge: move_back\n");
			if(count != 1)
			{
				//stop();
				//DelayMS(1000);
				turn(forward_speed,move_backward);
				//printf("forward_speed: %d\n",forward_speed);
				DelayMS(100);
			}
			else
			{
				turn(1023,move_backward);
				DelayMS(330);
			}
			count  = 2;
			
			turn_dir = 3;//turn left by default
			if(left_edge == true)
				turn_dir= 2;
		}
	}
	else
	{
		count = 1;
		delay_speed = 1;
		if(turn_dir != -1)
		{
			turn(800,turn_dir);
			DelayMS(300);
			turn_dir = -1;
			//printf("edge, turn! turn_dir = %d\n", turn_dir);
		}
		else
		{
			if(my_eyes.Get_Obs_information(obs_information) == true)
			{
				int temp_turn = 0;
				int turn_time = 0;
				int heat_dir = 0;
				if(obs_information[LEFT_FRONT] == true || obs_information[RIGHT_FRONT] == true)
				
					turn_time = 150;
				
				if(obs_information[RIGHT] == true || obs_information[LEFT] == true)
					turn_time = 300;
				
				if(obs_information[RIGHT] == true || obs_information[RIGHT_FRONT] == true)
					temp_turn = 2;
				if(obs_information[LEFT_FRONT] == true || obs_information[LEFT] == true)
					temp_turn = 3;

				if(obs_information[LEFT_FRONT] == true && obs_information[RIGHT_FRONT] == true)
				{
					temp_turn = 0;
					turn_time = 0;
				}

				if(obs_information[RIGHT] == true && obs_information[RIGHT_FRONT] == true)
				{
					turn_time = 200;
				}
				if(obs_information[LEFT_FRONT] == true && obs_information[LEFT] == true)
				{
					turn_time = 200;
				}
				if(obs_information[BACK] == true)
				{
					heat_dir = 1;
					turn_time = 0;
					//turn_dir = 0;
				}
				if(obs_information[FRONT] == true)
				{
					turn_time = 0;
				}/*
				if(temp_turn == 2)
					//printf("turn_right ");
				
				else if(temp_turn == 3)
					//printf("turn_left ");
				else if(heat_dir== 1)
					//printf("move_back to hit\n");

				if(turn_time == 150)
					//printf("45 angle\n");

				else if(turn_time ==200)
					//printf("75 angle\n");

				else if(turn_time ==300)
					//printf("90 angle\n");
					*/
				if(turn_time != 0)
				{
					turn(400,temp_turn);
					DelayMS(turn_time);
				}
				turn(400,heat_dir);
				//printf("heat!\n");
				DelayMS(20);

			}
			else
			{
				turn(400,0);
				//printf("running normally\n");
				DelayMS(20);
			}
		}
	}
}

void Suzaku::brain()
{
	turn_dir = -1;
	while(1)
	{
		Edge_dection();
	}
}

