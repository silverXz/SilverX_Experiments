#pragma once

#include <glm/glm.hpp>


class WxCamera
{
public:
	WxCamera();
	WxCamera(glm::vec3,glm::vec3,glm::vec3);

	//Camera Attribute
	void setPosition(glm::vec3);
	void setFocus(glm::vec3);
	void setUpOrien(glm::vec3);
	void reset(void);
	void setFrustrum(float xangle,float xratio,float xnear,float xfar);

	glm::vec3 getPosition(void)
	{
		return mposition; 
	}

	glm::vec3 getUpOrien(void)
	{
		return mUp;
	}

	glm::vec3 getLookOrien(void)
	{
		return glm::vec3(glm::normalize(mlookAt - mposition));
	}

	glm::vec3 getFocus(void)
	{
		return mlookAt;
	}

	//Camera Operation

	void zoomIn(void);
	void zoomOut(void);
	void cwRotate(void);
	void ccwRotate(void);
	void moveUp(void);
	void moveDown(void);
	glm::mat4 getViewMatrix(void){ return view;}
	glm::mat4 getProjectionMatrix(void) { return projection;}
	float getCurrentAngle();
	void setCurrentAngle(float _angle);


private:
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 mposition;
	glm::vec3 mlookAt;
	glm::vec3 mUp;
	float curAngle;
	float focalLength;

	static float zSpeed;
	static float rSpeed;
	static float tSpeed;
};
