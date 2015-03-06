#pragma once

#include <windowsx.h>
#include <Windows.h>
#include <glm/glm.hpp>

//Show the monitor information on console.
void ShowMonitorInfo(void);


struct ClientRect
{
	int x;
	int y;
	int width;
	int height;

	ClientRect(int a,int b,int c,int d):x(a),y(b),width(c),height(d)
	{}
};

typedef struct{
	glm::vec3 camPos;
	glm::vec3 camLookAt;
	glm::vec3 camRight;
	glm::vec3 camUp;
} Camera;

typedef struct {						//Structure For Keyboard Stuff
	BOOL keyDown[256];			// Holds TRUE / FALSE For Each Key
} Keys;									//Keys
