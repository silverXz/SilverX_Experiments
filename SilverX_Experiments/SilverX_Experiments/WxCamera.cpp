#include "stdafx.h"
#include "WxCamera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"

const float Pai = 3.14159265f;
const float halfPai = Pai / 2.0f;



using namespace glm;

WxCamera::WxCamera():curAngle(0.0)
{
	mposition = vec3(0.0,0.0,10.0);
	mlookAt = vec3(0.0,0.0,0.0);
	mUp = vec3(0.0,1.0,0.0);
	view = glm::lookAt(mposition,mlookAt,mUp);
};

WxCamera::WxCamera(vec3 loc,vec3 foc,vec3 up):curAngle(0.0)
{
	mposition = loc;
	mlookAt = foc;
	mUp = up;
	view = glm::lookAt(mposition,mlookAt,mUp);
}

void WxCamera::setPosition(vec3 pos)
{
	mposition = pos;
	view = glm::lookAt(mposition,mlookAt,mUp);
}

void WxCamera::setFocus(vec3 focus)
{
	mlookAt = focus;
	view = glm::lookAt(mposition,mlookAt,mUp);
}

void WxCamera::setUpOrien(vec3 upup)
{
	mUp = upup;
	view = glm::lookAt(mposition,mlookAt,mUp);
}

void WxCamera::reset()
{
	mposition = vec3(0.0,0.0,10.0);
	mlookAt = vec3(0.0,0.0,0.0);
	mUp = vec3(0.0,1.0,0.0);
	view = glm::lookAt(mposition,mlookAt,mUp);
}

void WxCamera::zoomIn()
{
	vec4 Orient = normalize(vec4(mlookAt - mposition,1.0));
	vec3 mov = vec3(Orient.x,Orient.y,Orient.z) * zSpeed;
	mposition += mov;
	view = glm::lookAt(mposition,mlookAt,mUp);
}


void WxCamera::zoomOut()
{
	vec4 Orient = normalize(vec4(mlookAt - mposition,1.0));
	vec3 mov = vec3(Orient.x,Orient.y,Orient.z) * zSpeed;
	mposition -= mov;
	view = glm::lookAt(mposition,mlookAt,mUp);
}

void WxCamera::cwRotate()
{
	
	if((curAngle + rSpeed) > halfPai / 2.0f)
		return;
	else
		curAngle += rSpeed;
	vec4 last = vec4(mposition,1.0);
	mat4 rMat = glm::rotate(mat4(1.0),glm::degrees(rSpeed),vec3(0.0,1.0,0.0));
	mposition = vec3(rMat*last);
	view = glm::lookAt(mposition,mlookAt,mUp);
}

void WxCamera::ccwRotate()
{
	if((curAngle - rSpeed) < -halfPai / 2.0f)
		return;
	else
		curAngle -= rSpeed;
	vec4 last = vec4(mposition,1.0);
	mat4 rMat = glm::rotate(mat4(1.0),glm::degrees(-rSpeed),vec3(0.0,1.0,0.0));
	mposition = vec3(rMat*last);
	view = glm::lookAt(mposition,mlookAt,mUp);
}

void WxCamera::moveUp()
{
	vec3 orien(0.0,1.0,0.0);
	mlookAt += tSpeed * orien;
	mposition += tSpeed * orien;
	view = glm::lookAt(mposition,mlookAt,mUp);
}

void WxCamera::moveDown()
{
	vec3 orien(0.0,-1.0,0.0);
	mlookAt += tSpeed * orien;
	mposition += tSpeed * orien;
	view = glm::lookAt(mposition,mlookAt,mUp);
}

void WxCamera::setFrustrum(float xangle,float xratio,float xnear,float xfar)
{
	projection = glm::perspective(xangle,xratio,xnear,xfar);
}

float WxCamera::getCurrentAngle()
{
	return curAngle;
}

void WxCamera::setCurrentAngle( float _angle )
{
	curAngle = _angle;
}


float WxCamera::zSpeed = 0.10f;
float WxCamera::rSpeed = halfPai / 40.0f ;   //9 degree one time
float WxCamera::tSpeed = 0.18f;