#pragma once

#include "SiMath.h"


class WxCamera
{
public:
	WxCamera();
	WxCamera(float eye_x,float eye_y,float eye_z,float at_x,float at_y,float at_z,float up_x,float up_y,float up_z);

	WxCamera(const SilverX::Vector3f& eye, const SilverX::Vector3f& lookat,const SilverX::Vector3f& up);

	//Camera Attribute
	void setCamPosition(SilverX::Vector3f p);
	void setSilUpOrien(SilverX::Vector3f u);
	void setFocus(SilverX::Vector3f lookat);
	void setUpOrien(SilverX::Vector3f up);
	
	
	void reset(void);
	void setFrustrum(float xangle,float xratio,float xnear,float xfar);


	SilverX::Vector3f getCamPosition();



	SilverX::Vector3f getUpOrien(void);

	SilverX::Vector3f getLookOrien(void);

	//Camera Operation

	void zoomIn(void);
	void zoomOut(void);
	void cwRotate(void);
	void ccwRotate(void);

	void moveUp(void);
	void moveDown(void);
	SilverX::Matrix4f getViewMatrix();
	SilverX::Matrix4f getProjectionMatrix(void);


	float getCurrentAngle();
	void setCurrentAngle(float _angle);


private:
	SilverX::Matrix4f vMat;
	SilverX::Matrix4f projMat;
	SilverX::Vector3f camPos;
	SilverX::Vector3f camLookAt;
	SilverX::Vector3f camUp;
	float curAngle;
	float focalLength;

	static float zSpeed;
	static float rSpeed;
	static float tSpeed;
	static float forwardSpeed;
};
