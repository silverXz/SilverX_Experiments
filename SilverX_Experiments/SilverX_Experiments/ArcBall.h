#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "SiMath.h"

class WxCamera;


class ArcBall
{
public:
	ArcBall(WxCamera* pCamera,int width,int height);
	~ArcBall();

	void attachCamera(WxCamera* pCamera);
	WxCamera* getCameraPtr();
	void setViewPort(int w,int h);


	void MoveSilverX(int x,int y);
	void pinchLeft(int x, int y);
	void releaseLeft(int x,int y);
	void pinchRight(int x, int y);
	void releaseRight(int x,int y);

	glm::vec3 get_arcball_vector(int,int);

	SilverX::Vector3f Get_ArcBall_Vector(int x,int y);

private:

	WxCamera* m_pCamera;
	
	int m_width,m_height;
	int m_curLeftX,m_curLeftY;
	int m_preLeftX,m_preLeftY;

	int m_curRightX,m_curRightY;
	int m_preRightX,m_preRightY;

	bool m_bLeftButtonDown;
	bool m_bRightButtonDown;
};