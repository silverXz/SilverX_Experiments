#include "utility.h"
#include <iostream>

//This function show the information of the monitors which are active in current operating system.
//For the purpose of further use of multiple screen display.
void ShowMonitorInfo()
{
	printf("**************************************************************\n");
	printf("Monitor Information...\n");
	int numMonitor = GetSystemMetrics(SM_CMONITORS);
	printf("There are currently %d active monitor...\n",numMonitor);
	//Get Primary Monitor Information
	int width = 0;
	int height = 0;
	POINT pt;
	pt.x = 0;
	pt.y = 0;
	HMONITOR hPrimaryMonitor = MonitorFromPoint(pt,MONITOR_DEFAULTTONEAREST);
	//GetMonitorInfo Function:
	//Param @HMONITOR hMonitor : A handle to the display monitor of interest
	//Param @LPMONITORINFO lpmi: A pointer to a MONITORINFO or MONITORINFOEX structure that receives information about the 
	//specified display monitor. You must set the cbSize member of the structure to sizeof(MONITORINFO) or sizeof(MONITORINFOEX)
	//before calling the GetMonitorInfo function. Doing so lets the function determine the type of structure you are passing to it.
	if(hPrimaryMonitor != NULL)
	{
		//MONITORINOF structure:
		//  @cbSize : The size of the structure, in bytes
		//  @rcMonitor : A RECT structure that specifies the display monitor rectangle, expressed in virtual-screen coordinates. Note that
		//						if the monitor is not the primary display monitor, some of the rectangle's coordinates may be negative values.
		//  @rcWork:		A RECT structure that specifies the work area rectangle of the display monitor, expressed in virtual-screen coordinates
		//						Note that  if the monitor is not the primary display monitor, some of the rectangles' coordinates may be negative values
		//  @dwFlags:		A set of flags that represent attributes of the display monitor.

		//RECT Structure:
		//@left:		The x-coordinate of the upper-left corner of the rectangle
		//@top:		The y-coordinate of the upper-left corner of the rectangle
		//@right:	The x-coordinate of the lower-right corner of the rectangle
		//@bottom: The y_coordinate of the lower-right corner of the rectangle
		MONITORINFO mi;
		mi.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hPrimaryMonitor,&mi);
		width = (int)(mi.rcMonitor.right - mi.rcMonitor.left);
		height = (int)(mi.rcMonitor.bottom - mi.rcMonitor.top);
		printf("The primary monitor's resolution is %d * %d...\n",width,height);
	}

	//Get the secondary Monitor Information:
	if( numMonitor <= 1)
	{
		printf("**************************************************************\n");
		return;
	}
	pt.x += (int)(width * 1.5);
	pt.y += height /2 ;
	HMONITOR hSecondaryMonitor = MonitorFromPoint(pt,MONITOR_DEFAULTTONEAREST);
	if(hSecondaryMonitor == hPrimaryMonitor)
	{
		printf("**************************************************************\n");
		return;
	}
	if(hSecondaryMonitor != NULL)
	{
		MONITORINFO mi;
		mi.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hSecondaryMonitor,&mi);
		width = (int)(mi.rcMonitor.right - mi.rcMonitor.left);
		height = (int)(mi.rcMonitor.bottom - mi.rcMonitor.top);
		printf("The secondary monitor's resolution is %d * %d...\n",width,height);
	}
	printf("**************************************************************\n");
}