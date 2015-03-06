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

	double GetElapsedTime();            // 秒            
	double GetElapsedTimeInMilliSec();  // 毫秒 
	double GetElapsedTimeInMicroSec();  // 微秒            

	// 1秒 = 10的3次方毫秒 = 10的6次方微秒;
};

