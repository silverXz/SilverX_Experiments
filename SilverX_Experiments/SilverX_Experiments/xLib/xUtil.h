// xUtil.h
// ***************************************************************
//  xUtil   version:  1.0   ?
//  -------------------------------------------------------------
//  Description:  utils for Math.
//  Author     :  wujh@sia.cn 
//  Created    :  29:4:2006   15:58
//  Modified   : 
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#if !defined _UTIL_H__INCLUDED_
#define _UTIL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <cmath>

#define  Deg2Rad(a) (a*0.017453292519943295)
#define  Rad2Deg(a) (a*57.29577951308232157)

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2  1.57079632679489661923
#endif
#ifndef M_2_PI
#define M_2_PI  (2*M_PI)
#endif

const double  EPSILON  = 1E-6;

// 求最大值
template<typename T>
inline const T& Max(const T& x, const T& y, const T& z)
{
	return (x > y) ? (x > z ? x : z) : (y > z ? y : z);
}

// 求最小值
template<typename T>
inline const T& Min(const T& x, const T& y, const T& z)
{
	return (x < y) ? (x < z ? x : z) : (y < z ? y : z);
}


// 求最大值
template<typename T>
inline const T& Max(const T& x, const T& y)
{
	return (x > y ? x : y);
}


// 求最小值
template<typename T>
inline const T& Min(const T& x, const T& y)
{
	return (x < y ? x : y);
}
namespace MrWu
{
template<typename T>
inline void swap(T& x, T& y)
{
	T temp = x;
	x = y;
	y = temp;
}
}
double GaussianRandom();

// Quick 'n dirty portable random number generator 
inline float tinyrnd()
{
	static unsigned trand = 0;
	trand = 1664525u * trand + 1013904223u;
	return (float) trand / 4294967296.0f;
}

bool IsZero(float x);

template<typename T>
inline bool IsZero(T x)
{
	if ( fabs(x) < EPSILON )
		return true;
	else
		return false;
}

inline bool IsEqual(float x, float y, float epsilon = EPSILON)
{
	if ( fabs(x - y ) < epsilon )
		return true;
	else
		return false;
}


inline float Round(float val)	
{
	return powf(10.0f, ceil(log10(val)) );
}

inline double safeAcos(double x)
{
	if (x <= -1.0f)
	{
		return M_PI;
	}
	if (x >= 1.0f)
	{
		return 0.0f;
	}

	return acos(x);
}



//*******************************************
// User message in status bar
//*******************************************
void StatusMessage(char* fmt,...);





#endif // !defined(_UTIL_H__INCLUDED_)
