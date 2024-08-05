#include <cassert>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "servo.h"
#include "config.h"
#include "basic.h"
// #include "test.h"
#include "mission.h"
#include "suzaku.h"
#include "test_brake.h"
#include "flag.h"

//static Config & C = Config::getInstance();
const int IO_PATTERN = 0x000;

int main(){
	init();
	Flag::init();
	printf("ggfuckdsd2r21\n");
	MFSetPortDirect(IO_PATTERN);
	DelayMS(100);
	//MissionManager msm("main_manager");
	//msm.init();
	Suzaku suzaku;
	while(1)
	{
		if(MFGetDigiInput(3) == 0)
			break;
		DelayMS(50);
	}
	suzaku.brain();
	print("quit\n");
	quit();
	DelayMS(3000);
	return 0;
}
