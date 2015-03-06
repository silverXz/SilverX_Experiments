#include "stdafx.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"

#include "ShaderLib.h"
#include "WxCamera.h"
#include "xLib/xMesh.h"
#include "xLib/xFileIO.h"
#include "GLShader.h"
#include "Quad.h"

SilverScene::SilverScene()
	:m_width(800),
	m_height(600),
	m_pShaderLib(NULL),
	m_pMesh(NULL),
	m_hFBO(0),
	m_hAO(0),
	last(),
	cur(),
	m_pCamera(NULL),
	m_pQuad(NULL),
	m_bPinched(false),
	m_bSceneInited(false),
	m_bFBOInited(false)
{
}

SilverScene::~SilverScene()
{
	if( m_pShaderLib )
		delete m_pShaderLib,m_pShaderLib = NULL;
	if( m_pMesh )
		delete m_pMesh, m_pMesh = NULL;
	if( m_pCamera )
		delete m_pCamera, m_pCamera = NULL;
	if( m_pQuad )
		delete m_pQuad, m_pQuad = NULL;
	if( m_hFBO > 0 )
		glDeleteFramebuffers(1,&m_hFBO);
	if( m_hAO > 0 )
		glDeleteFramebuffers(1,&m_hAO);
}

void SilverScene::Init()
{
	if( m_bSceneInited )
		return;
	m_pShaderLib = new ShaderLib;
	m_pShaderLib->CompileAndLinkAllShaderPrograms();

	m_pQuad = new Quad;

	m_pCamera = new WxCamera;

	glClearColor(0.0,0.0,0.0,1.0);
	glEnable(GL_DEPTH_TEST);

	//SetupFramebuffer();


	m_bSceneInited = true;
}

void SilverScene::RenderScene()
{

	if( !m_bSceneInited )
		return;
	if( m_pMesh == NULL )
		return;

	GLShader* pShader = m_pShaderLib->GetShaderProgram(E_GENERAL_SHADER);
	pShader->Use();


	glBindFramebuffer(GL_FRAMEBUFFER,0);  // Render to back buffer
	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	glm::mat4 modelMatrix(1.0f);
	glm::mat4 viewMatrix = m_pCamera->getViewMatrix();
	glm::mat4 projectionMatrix = glm::perspective(90.0f,static_cast<float>(m_width)/static_cast<float>(m_height),0.1f,1000.0f);
	glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

	pShader->SetUniform("ModelMatrix",modelMatrix);
	pShader->SetUniform("ViewMatrix",viewMatrix);
	pShader->SetUniform("MVP",MVP);

	glm::vec4 LightWorldPosition(50.0f,50.0f,50.0f,1.0f);
	glm::vec3 LightIntensity(0.45f);
	pShader->SetUniform("Light0.Position",glm::vec3(viewMatrix * LightWorldPosition));
	pShader->SetUniform("Light0.Intensity",LightIntensity);

	pShader->SetUniform("material.Ka",glm::vec3(0.3f));
	pShader->SetUniform("material.Kd",glm::vec3(0.7f));
	pShader->SetUniform("material.Ks",glm::vec3(0.7f));

	pShader->SetUniform("material.Shininess",128.0f);

	m_pMesh->render();
}

void SilverScene::Pinch(int x,int y)
{
	last.x = x;
	last.y = y;
	m_bPinched = true;
}

void SilverScene::Move(int x, int y)
{
	if( !m_bSceneInited )
		return;
	if(!m_bPinched)
		return;
	cur.x = x;
	cur.y = y;
	if(last == cur)
		return;
	glm::vec3 va = Get_Arcball_Vector(last.x,last.y);
	glm::vec3 vb = Get_Arcball_Vector(cur.x,cur.y);
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

	last = cur;
}

void SilverScene::Release(int x, int y)
{
	last.x = x;
	last.y = y;
	m_bPinched = false;
}

glm::vec3 SilverScene::Get_Arcball_Vector(int x,int y)
{
	//注意！在计算arcball上的向量时，所采用的width和height应该是对鼠标移动起响应的那部分客户区，而不是整个客户区。
	//也就是说，这里的宽和高应该是splitView时为MainView和XrayView分配的那部分View的长和宽。
	//glm::vec3 P = glm::vec3(2.0*(float)x / (float)width - 1.0, 2.0*(float)y /(float)height - 1.0 , 0.0);

	glm::vec3 P = glm::vec3(2.0*(float)x / m_width - 1.0, 2.0*(float)y / m_height - 1.0 , 0.0);
	P.y = - P.y;
	float OP_squared = P.x*P.x + P.y * P.y;
	if(OP_squared <= 1.0)
		P.z = sqrt(1.0f - OP_squared);
	else
		P = glm::normalize(P);
	return P;
}

void SilverScene::Resize( int w,int h )
{
	m_width = w;
	m_height = h;
}

void SilverScene::LoadMesh( const char* filePath )
{
	if( m_pMesh != NULL )
		delete m_pMesh;
	m_pMesh = CxFileIO::ReadFile(filePath);
	m_pMesh->CreateAdjacency();
	m_pMesh->CalculateNormalPerFace();
	m_pMesh->CalculateNormalPerVertex();
	m_pMesh->CalculateBoundingBox();

	m_pCamera->reset();
	m_pCamera->setPosition(glm::vec3(0.0,0.0,0.5 * m_pMesh->GetZRange()));
}

void SilverScene::ZoomIn()
{
	assert(m_pCamera != NULL);
	m_pCamera->zoomIn();
}

void SilverScene::ZoomOut()
{
	assert(m_pCamera != NULL);
	m_pCamera->zoomOut();
}

void SilverScene::SetupFramebuffer()
{
	if( m_bFBOInited )
		return;
	glGenFramebuffers(1,&m_hFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,m_hFBO);


	// Color Texture
	GLuint positionTexture = 0;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1,&positionTexture);
	glBindTexture(GL_TEXTURE_2D,positionTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,positionTexture,0);

	// Normal Texture
	GLuint normalTexture = 0;
	glActiveTexture(GL_TEXTURE1);			// Eye-Space Normal information will be save in GL_TEXTURE1
	glGenTextures(1,&normalTexture);
	glBindTexture(GL_TEXTURE_2D,normalTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,normalTexture,0);
	

	// ambient Texture
	GLuint ambientTexture = 0;			
	glActiveTexture(GL_TEXTURE2);		// [0,1] depth information will be save in GL_TEXTURE2
	glGenTextures(1,&ambientTexture);
	glBindTexture(GL_TEXTURE_2D,ambientTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,ambientTexture,0);

	// diffuse Texture
	GLuint diffuseTexture = 0;			
	glActiveTexture(GL_TEXTURE3);		// [0,1] depth information will be save in GL_TEXTURE2
	glGenTextures(1,&diffuseTexture);
	glBindTexture(GL_TEXTURE_2D,diffuseTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT3,GL_TEXTURE_2D,diffuseTexture,0);

	// Specular Texture
	GLuint specularTexture = 0;			
	glActiveTexture(GL_TEXTURE4);		// [0,1] depth information will be save in GL_TEXTURE2
	glGenTextures(1,&specularTexture);
	glBindTexture(GL_TEXTURE_2D,specularTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT4,GL_TEXTURE_2D,specularTexture,0);



	// Render buffer for depth test.
	GLuint depthRenderBuffer;
	glGenRenderbuffers(1,&depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER,depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,m_width,m_height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthRenderBuffer);

	GLenum drawbuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,GL_COLOR_ATTACHMENT4};

	glDrawBuffers(6,drawbuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if( result != GL_FRAMEBUFFER_COMPLETE) 
	{
		switch(result)
		{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			{
				printf("Failed! InComplete Attachment!\n");
				break;
			}
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			{
				printf("Failed! Incompelete Dimension!\n");
				break;
			}
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			{
				printf("Failed! Incomplete Missing Attachment!\n");
				break;
			}
		case GL_FRAMEBUFFER_UNSUPPORTED:
			{
				printf("Failed! Framebuffer Unsupported!\n");
				break;
			}
		default:
			break;
		}
	}
	else
		printf("Framebuffer Complete!\n");




	glGenFramebuffers(1,&m_hAO);
	glBindFramebuffer(GL_FRAMEBUFFER,m_hAO);

	// Color Texture
	GLuint aoTexture = 0;
	glActiveTexture(GL_TEXTURE5);
	glGenTextures(1,&aoTexture);
	glBindTexture(GL_TEXTURE_2D,aoTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,aoTexture,0);

	GLenum drawbuffers2[] = {GL_COLOR_ATTACHMENT0};

	glDrawBuffers(1,drawbuffers2);

	result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if( result != GL_FRAMEBUFFER_COMPLETE) 
	{
		switch(result)
		{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			{
				printf("Failed! InComplete Attachment!\n");
				break;
			}
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			{
				printf("Failed! Incompelete Dimension!\n");
				break;
			}
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			{
				printf("Failed! Incomplete Missing Attachment!\n");
				break;
			}
		case GL_FRAMEBUFFER_UNSUPPORTED:
			{
				printf("Failed! Framebuffer Unsupported!\n");
				break;
			}
		default:
			break;
		}
	}
	else
		printf("Framebuffer Complete!\n");


	glBindFramebuffer(GL_FRAMEBUFFER,0);

	m_bFBOInited = true;
}

void SilverScene::RenderSceneSSAO()
{
	if( !m_bSceneInited )
		return;
	if( m_pMesh == NULL )
		return;

	SetupFramebuffer();

	GLShader* pShader = m_pShaderLib->GetShaderProgram(E_SSAOP1_SHADER);
	pShader->Use();


	

	glBindFramebuffer(GL_FRAMEBUFFER,m_hFBO);  // Render to FBO

	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glm::mat4 modelMatrix(1.0f);
	glm::mat4 viewMatrix = m_pCamera->getViewMatrix();
	glm::mat4 projectionMatrix = glm::perspective(90.0f,static_cast<float>(m_width)/static_cast<float>(m_height),0.1f,1000.0f);
	glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

	pShader->SetUniform("ModelMatrix",modelMatrix);
	pShader->SetUniform("ViewMatrix",viewMatrix);
	pShader->SetUniform("MVP",MVP);

	pShader->SetUniform("material.Ka",glm::vec3(1.0f));
	pShader->SetUniform("material.Kd",glm::vec3(0.0f));
	pShader->SetUniform("material.Ks",glm::vec3(0.0f));

	m_pMesh->render();

	glBindFramebuffer(GL_FRAMEBUFFER,m_hAO); // Render to AO Buffer
	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	pShader = m_pShaderLib->GetShaderProgram(E_SSAOP2_SHADER);
	pShader->Use();



	pShader->SetUniform("TexPosition",0);		// FragColor
	pShader->SetUniform("TexNormal",1);		// FragNormal

	int max_Texel_Offset = 0;
	glGetIntegerv(GL_MIN_PROGRAM_TEXEL_OFFSET,&max_Texel_Offset);

	m_pQuad->render();

	pShader = m_pShaderLib->GetShaderProgram(E_SSAOP3_SHADER);
	pShader->Use();
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	pShader->SetUniform("TexPosition",0);		// FragColor
	pShader->SetUniform("TexNormal",1);		// FragNormal
	pShader->SetUniform("TexAmbient",2);		// FragDepth
	pShader->SetUniform("TexDiffuse",3);
	pShader->SetUniform("TexSpecular",4);
	pShader->SetUniform("TexAO",5);
	glm::vec4 LightWorldPosition(m_pMesh->GetBoundingBox()->xMax(),m_pMesh->GetBoundingBox()->yMax(),m_pMesh->GetBoundingBox()->zMax(),1.0f);
	glm::vec3 LightIntensity(0.75f);
	pShader->SetUniform("Light0.Position",glm::vec3(viewMatrix * LightWorldPosition));
	pShader->SetUniform("Light0.Intensity",LightIntensity);


	m_pQuad->render();


}

void SilverScene::RenderSceneMRT()
{
	if( !m_bSceneInited )
		return;
	if( m_pMesh == NULL )
		return;

	SetupFramebuffer();

	GLShader* pShader = m_pShaderLib->GetShaderProgram(E_MRT_SHADER);
	pShader->Use();




	glBindFramebuffer(GL_FRAMEBUFFER,m_hFBO);  // Render to FBO

	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glm::mat4 modelMatrix(1.0f);
	glm::mat4 viewMatrix = m_pCamera->getViewMatrix();
	glm::mat4 projectionMatrix = glm::perspective(90.0f,static_cast<float>(m_width)/static_cast<float>(m_height),0.01f,1000.0f);
	glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

	pShader->SetUniform("ModelMatrix",modelMatrix);
	pShader->SetUniform("ViewMatrix",viewMatrix);
	pShader->SetUniform("MVP",MVP);

	pShader->SetUniform("material.Ka",glm::vec3(0.7f));
	pShader->SetUniform("material.Kd",glm::vec3(0.4f));
	pShader->SetUniform("material.Ks",glm::vec3(0.3f));

	pShader->SetUniform("material.Shininess",128.0f);

	m_pMesh->render();

	glBindFramebuffer(GL_FRAMEBUFFER,0); // Render to back buffer
	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	pShader = m_pShaderLib->GetShaderProgram(E_QUAD_SHADER);
	pShader->Use();

	glm::vec4 LightWorldPosition(m_pMesh->GetBoundingBox()->xMax(),m_pMesh->GetBoundingBox()->yMax(),m_pMesh->GetBoundingBox()->zMax(),1.0f);
	glm::vec3 LightIntensity(0.75f);
	pShader->SetUniform("Light0.Position",glm::vec3(viewMatrix * LightWorldPosition));
	pShader->SetUniform("Light0.Intensity",LightIntensity);

	pShader->SetUniform("TexPosition",0);		// FragColor
	pShader->SetUniform("TexNormal",1);		// FragNormal
	pShader->SetUniform("TexAmbient",2);		// FragDepth
	pShader->SetUniform("TexDiffuse",3);
	pShader->SetUniform("TexSpecular",4);

	int max_Texel_Offset = 0;
	glGetIntegerv(GL_MIN_PROGRAM_TEXEL_OFFSET,&max_Texel_Offset);

	m_pQuad->render();
}
