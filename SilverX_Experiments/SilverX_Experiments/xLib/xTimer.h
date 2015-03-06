#pragma once

//--------------------------------------------------------//
//  USEAGE:
//  CxTimer timer;
//  timer.Start();
//  // do something
//  .....
//  timer.Stop();
//  double elapsedTime = timer.GetElapsedTimeInMilliSec();
//
//--------------------------------------------------------//

#include <Windows.h>

class CxTimer
{
private:

	double		mStartTimeInMicroSec;   // starting time in micro-second
	double		mEndTimeInMicroSec;     // ending time in micro-second
	bool		mStoppedFlag;           // stop flag 

	LARGE_INTEGER mFrequency;           // ticks per second
	LARGE_INTEGER mStartCount;                   
	LARGE_INTEGER mEndCount;    


public:
	CxTimer(void);
	~CxTimer(void);

	void Start();                      
	void Stop(); 

	double GetElapsedTime();            // ��            
	double GetElapsedTimeInMilliSec();  // ���� 
	double GetElapsedTimeInMicroSec();  // ΢��            

	// 1�� = 10��3�η����� = 10��6�η�΢��;
};

