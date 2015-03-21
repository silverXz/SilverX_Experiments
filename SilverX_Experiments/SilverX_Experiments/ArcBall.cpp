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



SilverX::Vector3f ArcBall::Get_ArcBall_Vector( int x,int y )
{
	SilverX::Vector3f P(2.0f*(float)x / m_width - 1.0f, 2.0f*(float)y / m_height - 1.0f , 0.0f);
	P.y() = - P.y();
	float OP_squared = P.x()*P.x() + P.y() * P.y();
	if(OP_squared <= 1.0)
		P.z() = sqrt(1.0f - OP_squared);
	else
		P = SilverX::normalize(P);
	return P;
}

void ArcBall::MoveSilverX( int x,int y )
{
	if( !m_bLeftButtonDown && !m_bRightButtonDown )
		return;

	m_curLeftX = x; m_curLeftY = y;
	m_curRightX = x; m_curRightY = y;


	if( m_bLeftButtonDown && !m_bRightButtonDown )
	{	
		if(m_preLeftX == m_curLeftX && m_preLeftY == m_curLeftY )
			return;
		SilverX::Vector3f va = Get_ArcBall_Vector(m_preLeftX,m_preLeftY);
		SilverX::Vector3f vb = Get_ArcBall_Vector(m_curLeftX,m_curLeftY);
		float rotateRadius = 1.5f* acos(SilverX::sil_min(1.0f,SilverX::dot(va,vb)));
		SilverX::Vector3f axis_in_camera_coord = SilverX::cross(va,vb);

		// camViewMatrix From World to Eye Coordinates
		SilverX::Matrix4f camViewMatrix = m_pCamera->getViewMatrix();
		// cam2world : From Eye Coordinate to World Coordinate
		SilverX::Matrix3f cam2world = SilverX::inverse(SilverX::Matrix3f(camViewMatrix));
		SilverX::Vector3f axis_in_world = cam2world * axis_in_camera_coord;
		SilverX::Vector3f camWorldPos = m_pCamera->getCamPosition();
		SilverX::Vector3f camWorldUp = m_pCamera->getUpOrien();
		SilverX::Matrix4f rotx = SilverX::rotate(-rotateRadius,axis_in_world);
		camWorldUp = SilverX::Matrix3f(rotx) * camWorldUp;

		camWorldPos = SilverX::Vector3f(rotx * SilverX::Vector4f(camWorldPos,1.0));
		m_pCamera->setCamPosition(camWorldPos);
		m_pCamera->setUpOrien(camWorldUp);

		m_preLeftX = m_curLeftX; m_preLeftY = m_curLeftY;
		return;
	}
}
