#include "StdAfx.h"
#include "xTimer.h"


CxTimer::CxTimer(void)
{
	QueryPerformanceFrequency(&mFrequency);
	this->mStartCount.QuadPart = 0;
	this->mEndCount.QuadPart = 0;

	mStoppedFlag = 0;
	mStartTimeInMicroSec = 0;
	mEndTimeInMicroSec = 0;

}


CxTimer::~CxTimer(void)
{
}

void CxTimer::Start()
{
	mStoppedFlag = 0; // reset stop flag
	QueryPerformanceCounter(&mStartCount);
}

void CxTimer::Stop()
{
	mStoppedFlag = 1; // set timer stopped flag
	QueryPerformanceCounter(&mEndCount);
}

double CxTimer::GetElapsedTimeInMicroSec()
{
	if ( !mStoppedFlag )
		QueryPerformanceCounter(&mEndCount);

	mStartTimeInMicroSec = mStartCount.QuadPart * (1000000.0 / mFrequency.QuadPart);
	mEndTimeInMicroSec = mEndCount.QuadPart * (1000000.0 / mFrequency.QuadPart);

	return mEndTimeInMicroSec - mStartTimeInMicroSec;
}
// 1秒 = 10的3次方毫秒 = 10的6次方微秒;

double CxTimer::GetElapsedTimeInMilliSec()
{
	return this->GetElapsedTimeInMicroSec() * 0.001;
}

double CxTimer::GetElapsedTime()
{
	return this->GetElapsedTimeInMicroSec() * 0.000001;
}

