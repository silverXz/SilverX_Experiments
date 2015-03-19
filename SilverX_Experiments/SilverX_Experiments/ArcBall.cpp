#include "stdafx.h"
#include "ArcBall.h"
#include "WxCamera.h"


ArcBall::ArcBall( WxCamera* pCamera,int width,int height ) : m_pCamera(pCamera),m_width(width),m_height(height),
	m_bLeftButtonDown(false),m_bRightButtonDown(false)
{
}

ArcBall::~ArcBall()
{

}

void ArcBall::move( int x,int y )
{
	if( !m_bLeftButtonDown && !m_bRightButtonDown )
		return;

	m_curLeftX = x; m_curLeftY = y;
	m_curRightX = x; m_curRightY = y;



	if( m_bLeftButtonDown && !m_bRightButtonDown )
	{	
		if(m_preLeftX == m_curLeftX && m_preLeftY == m_curLeftY )
			return;
		glm::vec3 va = get_arcball_vector(m_preLeftX,m_preLeftY);
		glm::vec3 vb = get_arcball_vector(m_curLeftX,m_curLeftY);
		float rotateRadius = 1.5f* acos(glm::min(1.0f,glm::dot(va,vb)));
		glm::vec3 axis_in_camera_coord = glm::cross(va,vb);

		// camViewMatrix From World to Eye Coordinates
		glm::mat4 camViewMatrix = m_pCamera->getViewMatrix();
		// cam2world : From Eye Coordinate to World Coordinate
		glm::mat3 cam2world = glm::inverse(glm::mat3(camViewMatrix));
		glm::vec3 axis_in_world = cam2world * axis_in_camera_coord;
		glm::vec3 camWorldPos = m_pCamera->getPosition();
		glm::vec3 camWorldUp = m_pCamera->getUpOrien();
		glm::mat4 rotx = glm::rotate(glm::mat4(1.0),-glm::degrees(rotateRadius),axis_in_world);
		camWorldUp = glm::mat3(rotx) * camWorldUp;
		camWorldPos = glm::vec3(rotx * glm::vec4(camWorldPos,1.0));
		m_pCamera->setPosition(camWorldPos);
		m_pCamera->setUpOrien(camWorldUp);

		m_preLeftX = m_curLeftX; m_preLeftY = m_curLeftY;
		return;
	}
	//if( !m_bLeftButtonDown && m_bRightButtonDown )
	//{
	//	if( m_preRightY == m_curRightY )
	//		return;
	//	if( m_preRightY - m_curRightY > 0 )
	//		m_pCamera->zoomIn();
	//	else
	//		m_pCamera->zoomOut();
	//}

}

glm::vec3 ArcBall::get_arcball_vector( int x,int y)
{
	glm::vec3 P = glm::vec3(2.0*(float)x / m_width - 1.0, 2.0*(float)y / m_height - 1.0 , 0.0);
	P.y = - P.y;
	float OP_squared = P.x*P.x + P.y * P.y;
	if(OP_squared <= 1.0)
		P.z = sqrt(1.0f - OP_squared);
	else
		P = glm::normalize(P);
	return P;
}

void ArcBall::pinchLeft( int x, int y )
{
	m_preLeftX = x;		
	m_preLeftY = y;
	m_bLeftButtonDown = true;
}

void ArcBall::pinchRight( int x, int y )
{
	m_preRightX = x;	m_preRightY = y;
	m_bRightButtonDown = true;
}

void ArcBall::releaseLeft( int x,int y )
{
	m_preLeftX = x;
	m_preLeftY = y;
	m_bLeftButtonDown = false;
}

void ArcBall::releaseRight( int x,int y )
{
	m_preRightX = x;	m_preRightY = y;
	m_bRightButtonDown = false;
}

void ArcBall::setViewPort( int w,int h )
{
	m_width = w;
	m_height = h;
}

WxCamera* ArcBall::getCameraPtr()
{
	return m_pCamera;
}

void ArcBall::attachCamera( WxCamera* pCamera )
{
	m_pCamera = pCamera;
}

void ArcBall::moveForwards( void )
{
	if( !m_pCamera )
		return;
	m_pCamera->moveForwards();
}

void ArcBall::moveBackwards( void )
{
	if( !m_pCamera )
		m_pCamera->moveBackwards();
}
