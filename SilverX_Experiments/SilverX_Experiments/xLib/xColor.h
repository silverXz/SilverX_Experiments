// xColor.h: interface for the CxColor class.
// ***************************************************************
//  xColor   version:  1.0   
//  -------------------------------------------------------------
//  Description:  ÑÕÉ«Àà;
//  Author     :  wujh@sia.cn 
//  Created    :  07/17/2006
//  Modified   : 
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#if !defined _XCOLOR_H__INCLUDED_
#define _XCOLOR_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xUtil.h"
#include <cassert>

class CxColor  
{

private:
	unsigned char m_Red;
	unsigned char m_Green;
	unsigned char m_Blue;
	unsigned char m_Alpha;

public:
	
	// ctors
	CxColor(unsigned char r=0, unsigned char g=0, unsigned char b=0, unsigned a=0)
		                     :m_Red(r), m_Green(g), m_Blue(b), m_Alpha(a){ }
	CxColor(const CxColor& c):m_Red(c.m_Red), m_Green(c.m_Green), m_Blue(c.m_Blue),
		                      m_Alpha(c.m_Alpha) { }
	CxColor(unsigned char grey)  { Set(grey); }
	CxColor(unsigned char* rgb ) { m_Red = rgb[0]; m_Green = rgb[1]; m_Blue = rgb[2]; }
	CxColor(const COLORREF& cr);
	
	 ~CxColor() {}

	// Data access
	unsigned char r(void) const { return m_Red;    } 
	unsigned char g(void) const { return m_Green;  }  
	unsigned char b(void) const { return m_Blue;   }  
	unsigned char a(void) const { return m_Alpha;  }  

	// Data setting
	void r(unsigned char red)   { m_Red = red;     } 
	void g(unsigned char green) { m_Green = green; }  
	void b(unsigned char blue)  { m_Blue = blue;   }  
	void a(unsigned char alpha) { m_Alpha = alpha; }  

	void Set(const CxColor &c);
	void Set(unsigned char grey); 
	void Set(const COLORREF& cr);
	void Set(unsigned char r, unsigned char g, unsigned char b); 
	void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a); 
    void Set(float red, float green, float blue);
	
	// operator
	inline bool operator==(const CxColor& color)const;
	inline bool operator!=(const CxColor& color)const;

    // 
	static CxColor black()  { return CxColor(0, 0, 0);       }
	static CxColor white()  { return CxColor(255, 255, 255); }
	static CxColor red()    { return CxColor(255, 0, 0);     }
	static CxColor green()  { return CxColor(0, 255, 0);     }
	static CxColor blue()   { return CxColor(0, 0, 255);     }
	static CxColor yellow() { return CxColor(255, 255, 0);   }
	static CxColor cyan()   { return CxColor(0, 255, 255);   }
	static CxColor magenta(){ return CxColor(255, 0, 255);   }
	static CxColor dark_gray(){ return CxColor( (unsigned char)0.25*255, (unsigned char)0.25*255, (unsigned char)0.25*255);}
	static CxColor light_gray(){return CxColor((unsigned char)0.75*255, (unsigned char)0.75*255, (unsigned char)0.75*255);}
	static CxColor brown()  { return CxColor((unsigned char).60*255, (unsigned char).40*255, (unsigned char).12*255);}
	static CxColor pumpkin_orange() { return CxColor((unsigned char).98*255, (unsigned char).625*255, (unsigned char).12*255);} // ÄÏ¹Ï³È;
	static CxColor pastel_pink() { return CxColor((unsigned char).98*255, (unsigned char).04*255, (unsigned char).07*255);} // ·Ûºì;
	static CxColor barney_purple(){return CxColor((unsigned char).60*255, (unsigned char).40*255, (unsigned char).70*255);} // °ÍÄá×Ï;
#if 0
	static CxColor hsv(double h, double s, double v)
	{
		// From FvD
		if (s <= 0.0)
			return CxColor(v, v, v);
		h = fmod(h, 2.0 * M_PI);
		if (h < 0.0)
			h += 2.0 * M_PI;
		h /= M_PI / 3.0;
		int i = int(floor(h));
		double f = h - i;
		double p = v * (1.0 - s);
		double q = v * (1.0 - (s*f));
		double t = v * (1.0 - (s*(1.0-f)));
		switch(i) {
			case 0: return CxColor(v, t, p);
			case 1: return CxColor(q, v, p);
			case 2: return CxColor(p, v, t);
			case 3: return CxColor(p, q, v);
			case 4: return CxColor(t, p, v);
			default: return CxColor(v, p, q);
		}
	}
#endif


	void lerp(const CxColor& c0, const CxColor& c1, const float x )
	{
		assert( x>=0 );assert( x<=1 );

		m_Red   = (unsigned char)(x*c1.m_Red)   + (unsigned char)((1.0f - x)*c0.m_Red);
		m_Green = (unsigned char)(x*c1.m_Green) + (unsigned char)((1.0f - x)*c0.m_Green);
		m_Blue  = (unsigned char)(x*c1.m_Blue ) + (unsigned char)((1.0f - x)*c0.m_Blue );
		m_Alpha = (unsigned char)(x*c1.m_Alpha) + (unsigned char)((1.0f - x)*c0.m_Alpha);
	}

	void ColorRamp(const float& minf, const float& maxf, float v);
	void Trace()const;

};

inline void CxColor::ColorRamp(const float& minf, const float& maxf, float v)
{
	if( minf > maxf ) 
	{ 
		ColorRamp(maxf, minf, maxf+(minf-v) ); 
		return; 
	}  

	if(v <  minf ) 
	{ 
		*this = CxColor::red(); 
		return; 
	}

	//the case v > maxf is handled automatically at the end of the function
	float step = 0.25f*(maxf - minf); // /4;
	v -= minf;
	if ( v<step ) 
	{
		lerp(CxColor::red(), CxColor::yellow(), v/step);
		return;
	}

	v -= step;
	if ( v< step ) 
	{
		lerp(CxColor::yellow(), CxColor::green(), v/step);
		return;
	}

	v -= step;
	if ( v<step ) 
	{
		lerp(CxColor::green(), CxColor::cyan(), v/step);
		return;
	}

	v -= step;
	if ( v<step ) 
	{
		lerp(CxColor::cyan(), CxColor::blue(), v/step);
		return;
	}

	*this = CxColor::blue();
}


#endif // !defined(_XCOLOR_H__INCLUDED_)
