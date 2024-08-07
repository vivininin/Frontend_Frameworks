#pragma once
#include "servo.h"
#include "basic.h"

class Eyes{
	private:
		//Obsticle detection
		bool IsObs;
		//edge detection
		bool IsEdge;
	public:
		static const int UPFL = 9;
		static const int UPFR = 6;
		static const int UPBL = 7;
		static const int UPBR = 8;
			//Obsticle detection
		bool Get_Obs_information(bool information[]);
		//edge detection
		bool Get_Edge_information(bool information[]);
};


