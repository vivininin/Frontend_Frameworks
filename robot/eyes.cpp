#include "eyes.h"

//This function return a binary number, eg: 000 means there is nothing;
//111means there is Obstacle in the front,left,right side; so as it
bool Eyes::Get_Obs_information(bool information[])
{
	for(int i = 0; i < 6; i++)
		information[i] = false;
	//if(MFGetDigiInput(FRONT) == 0)
	bool is_obs = false;
	for(int i = 0; i <6; i++)
	{
		if(MFGetDigiInput(i) == 0)
		{
			information[i] = true;
			is_obs = true;
		}
	}
	return is_obs;
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
