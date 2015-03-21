#include "stdafx.h"
#include "WxCamera.h"

using namespace SilverX;
WxCamera::WxCamera():curAngle(0.0)
{
	camPos = Vector3f(0.0f,0.0f,10.0f);
	camLookAt = Vector3f(0.0f,0.0f,0.0f);
	camUp = Vector3f(0.0f,1.0f,0.0f);
	vMat = lookAt(camPos,camLookAt,camUp);
};

WxCamera::WxCamera( float eye_x,float eye_y,float eye_z,float at_x,float at_y,float at_z,float up_x,float up_y,float up_z )
	: camPos(eye_x,eye_y,eye_z), camLookAt(at_x,at_y,at_z),camUp(up_x,up_y,up_z)
{
	vMat = lookAt(camPos,camLookAt,camUp);
}

WxCamera::WxCamera( const SilverX::Vector3f& eye, const SilverX::Vector3f& lookat,const SilverX::Vector3f& up )
	: camPos(eye), camLookAt(lookat),camUp(up)
{
	vMat = lookAt(camPos,camLookAt,camUp);
}

void WxCamera::setCamPosition( SilverX::Vector3f p )
{
	camPos = p;
	vMat = lookAt(camPos,camLookAt,camUp);
}

void WxCamera::setUpOrien( SilverX::Vector3f u )
{
	camUp = u;
	vMat = lookAt(camPos,camLookAt,camUp);
}


void WxCamera::setFocus( SilverX::Vector3f lookat )
{
	camLookAt = lookat;
	vMat = lookAt(camPos,camLookAt,camUp);
}

void WxCamera::reset()
{
	camPos = Vector3f(0.0f,0.0f,10.0f);
	camLookAt = Vector3f(0.0f);
	camUp = Vector3f(0.0f,1.0f,0.0f);
	vMat = lookAt(camPos,camLookAt,camUp);
}

void WxCamera::zoomIn()
{
	Vector3f camOrient = normalize(camLookAt - camPos);
	camPos += camOrient * zSpeed;
	vMat = lookAt(camPos,camLookAt,camUp);
}


void WxCamera::zoomOut()
{
	Vector3f camOrient = normalize(camLookAt - camPos);
	camPos -= camOrient * zSpeed;
	vMat = lookAt(camPos,camLookAt,camUp);
}

void WxCamera::cwRotate()
{
	
	if((curAngle + rSpeed) > HALFPI / 2.0f)
		return;
	else
		curAngle += rSpeed;
	Vector4f last = Vector4f(camPos,1.0);
	Matrix4f rMat = rotate(rSpeed,Vector3f(0.0,1.0,0.0));
	camPos = Vector3f(rMat*last);
	vMat= lookAt(camPos,camLookAt,camUp);
}

void WxCamera::ccwRotate()
{
	if((curAngle - rSpeed) < -HALFPI / 2.0f)
		return;
	else
		curAngle -= rSpeed;
	Vector4f last = Vector4f(camPos,1.0);
	Matrix4f rMat = rotate(-rSpeed,Vector3f(0.0,1.0,0.0));
	camPos = Vector3f(rMat*last);
	vMat = lookAt(camPos,camLookAt,camUp);
}

void WxCamera::moveUp()
{
	Vector3f orien(0.0,1.0,0.0);
	camLookAt += orien * tSpeed;
	camPos += orien * tSpeed;
	vMat = lookAt(camPos,camLookAt,camUp);
}

void WxCamera::moveDown()
{
	Vector3f orien(0.0,-1.0,0.0);
	camLookAt += orien * tSpeed;
	camPos += orien * tSpeed;
	vMat = lookAt(camPos,camLookAt,camUp);
}

void WxCamera::setFrustrum(float xangle,float xratio,float xnear,float xfar)
{
	projMat = perspective(xangle,xratio,xnear,xfar);
}

float WxCamera::getCurrentAngle()
{
	return curAngle;
}

void WxCamera::setCurrentAngle( float _angle )
{
	curAngle = _angle;
}


SilverX::Matrix4f WxCamera::getViewMatrix()
{
	return vMat;
}

SilverX::Vector3f WxCamera::getLookOrien( void )
{
	return normalize(camLookAt - camPos);
}

SilverX::Vector3f WxCamera::getCamPosition()
{
	return camPos;
}

SilverX::Vector3f WxCamera::getUpOrien( void )
{
	return camUp;
}

SilverX::Matrix4f WxCamera::getProjectionMatrix( void )
{
	return projMat;
}



float WxCamera::zSpeed = 0.10f;
float WxCamera::rSpeed = HALFPI / 40.0f ;   //9 degree one time
float WxCamera::tSpeed = 0.18f;
float WxCamera::forwardSpeed = 0.10f;