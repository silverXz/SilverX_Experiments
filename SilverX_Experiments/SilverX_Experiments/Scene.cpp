#include "stdafx.h"
#include "Scene.h"

#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include "Box.h"
#include "ArcBall.h"
#include "ShaderLib.h"
#include "WxCamera.h"
#include "GLShader.h"
#include "Quad.h"
#include "SilMesh.h"

SilverScene::SilverScene()
	:m_width(800),
	m_height(600),
	m_pShaderLib(NULL),
	m_pSilMesh(NULL),
	m_pArcBall(NULL),
	m_hFBO(0),
	m_hAO(0),
	m_hBlur(0),
	m_pCamera(NULL),
	m_pQuad(NULL),
	m_pBox(NULL),
	m_bSceneInited(false),
	m_bFBOInited(false)
{
}

SilverScene::~SilverScene()
{
	if( m_pShaderLib )
		delete m_pShaderLib,m_pShaderLib = NULL;
	if( m_pCamera )
		delete m_pCamera, m_pCamera = NULL;
	if( m_pQuad )
		delete m_pQuad, m_pQuad = NULL;
	if( m_pArcBall )
		delete m_pArcBall, m_pArcBall = NULL;
	if( m_pBox )
		delete m_pBox, m_pBox = NULL;
	if( m_pSilMesh )
		delete m_pSilMesh, m_pSilMesh = NULL;
	if( m_hFBO > 0 )
		glDeleteFramebuffers(1,&m_hFBO);
	if( m_hAO > 0 )
		glDeleteFramebuffers(1,&m_hAO);
	if( m_hBlur > 0 )
		glDeleteFramebuffers(1,&m_hBlur);

}

void SilverScene::Init()
{
	if( m_bSceneInited )
		return;
	m_pShaderLib = new ShaderLib;
	m_pShaderLib->CompileAndLinkAllShaderPrograms();

	m_pQuad = new Quad;

	m_pCamera = new WxCamera;

	m_pBox = new Box;

	m_pArcBall = new ArcBall(m_pCamera,m_width,m_height);

	glClearColor(0.0,0.0,0.0,1.0);
	glEnable(GL_DEPTH_TEST);

	//SetupFramebuffer();
	std::srand(50);
	for( int i = 0 ; i < 16 ; i++ )
	{
		Samples[i].x() = 2.0f * ((float)std::rand() / RAND_MAX) - 1.0f;
		Samples[i].y() = 2.0f * ((float)std::rand() / RAND_MAX) - 1.0f;
		Samples[i].z() = (float)std::rand() / RAND_MAX;
		Samples[i] = SilverX::normalize(Samples[i]);
		/*float scale = (float)i / 16;
		float t = scale * scale;
		scale = 0.1f * (1.0f - t) + 1.0f * t;
		Samples[i] *= scale;*/
	}


	m_bSceneInited = true;
}

void SilverScene::RenderScene()
{
	if( !m_bSceneInited )
		return;
	if( m_pSilMesh == NULL )
		return;

	GLShader* pShader = m_pShaderLib->GetShaderProgram(E_GENERAL_SHADER);
	pShader->Use();


	glBindFramebuffer(GL_FRAMEBUFFER,0);  // Render to back buffer
	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	SilverX::Matrix4f modelMatrix(1.0f);
	SilverX::Matrix4f viewMatrix = m_pCamera->getViewMatrix();
	SilverX::Matrix4f projectionMatrix = SilverX::perspective(90.0f,static_cast<float>(m_width)/static_cast<float>(m_height),0.1f,1000.0f);
	SilverX::Matrix4f MVP = projectionMatrix * viewMatrix * modelMatrix;

	pShader->SetUniform("ModelMatrix",modelMatrix);
	pShader->SetUniform("ViewMatrix",viewMatrix);
	pShader->SetUniform("MVP",MVP);



	SilverX::Vector4f LightWorldPosition(100.0f,100.0f,100.0f,1.0f);
	SilverX::Vector3f LightIntensity(1.0f);
	pShader->SetUniform("Light0.Position",SilverX::Vector3f(viewMatrix * LightWorldPosition));
	pShader->SetUniform("Light0.Intensity",LightIntensity);

	pShader->SetUniform("material.Ka",SilverX::Vector3f(0.4f));
	pShader->SetUniform("material.Kd",SilverX::Vector3f(0.6f));
	pShader->SetUniform("material.Ks",SilverX::Vector3f(0.3f));

	pShader->SetUniform("material.Shininess",128.0f);

	m_pSilMesh->Render();

}


void SilverScene::Resize( int w,int h )
{
	m_width = w;
	m_height = h;
}

void SilverScene::LoadMesh( const char* filePath )
{
	if( m_pSilMesh )
		delete m_pSilMesh;
	m_pSilMesh = new SilMesh;
	m_pSilMesh->LoadMesh(filePath);
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



	// Normal Texture
	glActiveTexture(GL_TEXTURE0);			// Eye-Space Normal information will be save in GL_TEXTURE1
	glGenTextures(1,&m_hTexNormDepth);
	glBindTexture(GL_TEXTURE_2D,m_hTexNormDepth);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,m_width,m_height,0,GL_RGBA,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_hTexNormDepth,0);
	

	// ambient Texture
	GLuint ambientTexture = 0;			
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1,&ambientTexture);
	glBindTexture(GL_TEXTURE_2D,ambientTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,ambientTexture,0);

	// diffuse Texture
	GLuint diffuseTexture = 0;			
	glActiveTexture(GL_TEXTURE2);		// [0,1] depth information will be save in GL_TEXTURE2
	glGenTextures(1,&diffuseTexture);
	glBindTexture(GL_TEXTURE_2D,diffuseTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );


	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,diffuseTexture,0);

	// Specular Texture
	GLuint specularTexture = 0;			
	glActiveTexture(GL_TEXTURE3);		// [0,1] depth information will be save in GL_TEXTURE2
	glGenTextures(1,&specularTexture);
	glBindTexture(GL_TEXTURE_2D,specularTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT3,GL_TEXTURE_2D,specularTexture,0);



	// Render buffer for depth test.
	GLuint depthRenderBuffer;
	glGenRenderbuffers(1,&depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER,depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT32,m_width,m_height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthRenderBuffer);

	GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3};

	glDrawBuffers(4,drawbuffers);

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
	glActiveTexture(GL_TEXTURE4);
	glGenTextures(1,&aoTexture);
	glBindTexture(GL_TEXTURE_2D,aoTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
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


	glActiveTexture(GL_TEXTURE5);
	glGenTextures(1,&m_hRandomTex);
	glBindTexture(GL_TEXTURE_2D,m_hRandomTex);

	std::srand(100);
	glm::vec3 rVecs[4*4];
	for( int i = 0 ; i <  4*4 ; ++i )
	{
		rVecs[i].x = 2.0f * ((float)std::rand()/RAND_MAX) - 1.0f;
		rVecs[i].y = 2.0f * ((float)std::rand()/RAND_MAX) - 1.0f;
		rVecs[i].z = 0.0f;
		rVecs[i] = glm::normalize(rVecs[i]);
	}

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,4,4,0,GL_RGB,GL_FLOAT,rVecs);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );


	glGenFramebuffers(1,&m_hBlur);
	glBindFramebuffer(GL_FRAMEBUFFER,m_hBlur);

	glActiveTexture(GL_TEXTURE6);
	GLuint hBlurTex = 0;
	glGenTextures(1,&hBlurTex);
	glBindTexture(GL_TEXTURE_2D,hBlurTex);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,0);

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,hBlurTex,0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );

	GLenum drawbuffers3[] = {GL_COLOR_ATTACHMENT0};

	glDrawBuffers(1,drawbuffers3);

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
	if( m_pSilMesh == NULL )
		return;

	SetupFramebuffer();

	GLShader* pShader = m_pShaderLib->GetShaderProgram(E_SSAOP1_SHADER);
	pShader->Use();


	

	glBindFramebuffer(GL_FRAMEBUFFER,m_hFBO);  // Render to FBO

	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	SilverX::Matrix4f modelMat(1.0f);
	SilverX::Matrix4f vMat = m_pCamera->getViewMatrix();
	SilverX::Matrix4f projMat = SilverX::perspective(90.0f,static_cast<float>(m_width)/m_height,1.0f,1000.0f);
	SilverX::Matrix4f MVP = projMat * vMat * modelMat;

	pShader->SetUniform("ModelMatrix",modelMat);
	pShader->SetUniform("ViewMatrix",vMat);
	pShader->SetUniform("MVP",MVP);

	
	pShader->SetUniform("material.Ka",SilverX::Vector3f(1.0f));
	pShader->SetUniform("material.Kd",SilverX::Vector3f(0.0f));
	pShader->SetUniform("material.Ks",SilverX::Vector3f(0.0f));
	m_pSilMesh->Render();
	

	pShader->SetUniform("material.Ka",SilverX::Vector3f(1.0f));
	pShader->SetUniform("material.Kd",SilverX::Vector3f(0.3f));
	pShader->SetUniform("material.Ks",SilverX::Vector3f(0.0f));

	m_pBox->Render();






	glBindFramebuffer(GL_FRAMEBUFFER,m_hAO); // Render to AO Buffer
	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	pShader = m_pShaderLib->GetShaderProgram(E_SSAOP2_SHADER);
	pShader->Use();

	pShader->SetUniform("power",1.0f);

	SilverX::Matrix4f inverProjMat = SilverX::inverse(projMat);
	int loc = glGetUniformLocation(pShader->GetHandle(),"InverseProjectionMatrix");
	if( loc >= 0 )
		glUniformMatrix4fv(loc,1,GL_FALSE,inverProjMat.v);

	//glm::mat4 invP = glm::inverse(projectionMatrix);
	//pShader->SetUniform("InverseProjectionMatrix",invP);
	pShader->SetUniform("TexNormalDepth",0);		// FragColor
	pShader->SetUniform("TexRandom",5);
	pShader->SetUniform("ProjectionMatrix",projMat);
	pShader->SetUniform("NoiseScale",SilverX::Vector2f(m_width/4.0f,m_height/4.0f));
	pShader->SetUniform("Radius",0.1f);
	
	char UniName[20];
	for( int i = 0 ; i < 16 ; i++ )
	{
		sprintf_s(UniName,20,"Samples[%d]",i);
		pShader->SetUniform(UniName,Samples[i]);
	}

	int max_Texel_Offset = 0;
	glGetIntegerv(GL_MIN_PROGRAM_TEXEL_OFFSET,&max_Texel_Offset);

	m_pQuad->render();

	pShader = m_pShaderLib->GetShaderProgram(E_SSAOP3_SHADER);
	pShader->Use();
	glBindFramebuffer(GL_FRAMEBUFFER,m_hBlur);
	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	pShader->SetUniform("TexAO",4);

	m_pQuad->render();

	// Pass 4 Render to Screen
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	pShader = m_pShaderLib->GetShaderProgram(E_SSAOP4_SHADER);
	pShader->Use();
	
	//Vertex Shader
	pShader->SetUniform("InverseProjectionMatrix",SilverX::inverse(projMat));

	pShader->SetUniform("TexAmbient",1);
	pShader->SetUniform("TexDiffuse",2);
	pShader->SetUniform("TexSpecular",3);
	pShader->SetUniform("TexNormalDepth",0);
	pShader->SetUniform("TexAO",6);

	SilverX::Vector4f LightWorldPosition(0.0f,0.0f,0.0f,1.0f);
	SilverX::Vector3f LightIntensity(1.0f);
	pShader->SetUniform("Light0.Position",SilverX::Vector3f(vMat * LightWorldPosition));
	pShader->SetUniform("Light0.Intensity",LightIntensity);


	m_pQuad->render();


}

void SilverScene::RenderSceneMRT()
{
	if( !m_bSceneInited )
		return;
	if( m_pSilMesh == NULL )
		return;

	SetupFramebuffer();

	GLShader* pShader = m_pShaderLib->GetShaderProgram(E_MRT_SHADER);
	pShader->Use();




	glBindFramebuffer(GL_FRAMEBUFFER,m_hFBO);  // Render to FBO

	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	SilverX::Matrix4f modelMatrix(1.0f);
	SilverX::Matrix4f viewMatrix = m_pCamera->getViewMatrix();
	SilverX::Matrix4f projectionMatrix = SilverX::perspective(90.0f,static_cast<float>(m_width)/static_cast<float>(m_height),0.1f,1000.0f);
	SilverX::Matrix4f MVP = projectionMatrix * viewMatrix * modelMatrix;

	pShader->SetUniform("ModelMatrix",modelMatrix);
	pShader->SetUniform("ViewMatrix",viewMatrix);
	pShader->SetUniform("MVP",MVP);

	pShader->SetUniform("material.Ka",SilverX::Vector3f(0.7f));
	pShader->SetUniform("material.Kd",SilverX::Vector3f(0.4f));
	pShader->SetUniform("material.Ks",SilverX::Vector3f(0.3f));

	m_pSilMesh->Render();


	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D,m_hTexNormDepth);
	//
	//float* pixels = new float[ m_width * m_height * 4];
	//
	//glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,pixels);

	//for( int i = 0 ; i < m_width * m_height ; )
	//{
	//	printf("(%f,%f,%f) Depth: %f\n",pixels[i],pixels[i+1],pixels[i+2],pixels[i+3]);
	//	i += 4;
	//}



	//delete[] pixels;

	glBindFramebuffer(GL_FRAMEBUFFER,0); // Render to back buffer
	glViewport(0,0,m_width,m_height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	pShader = m_pShaderLib->GetShaderProgram(E_QUAD_SHADER);
	pShader->Use();

	SilverX::Vector4f LightWorldPosition(100.0f,100.0f,100.0f,1.0f);
	SilverX::Vector3f LightIntensity(0.75f);
	pShader->SetUniform("Light0.Position",SilverX::Vector3f(viewMatrix * LightWorldPosition));
	pShader->SetUniform("Light0.Intensity",LightIntensity);

	pShader->SetUniform("TexNormalDepth",0);		// FragNormal And Depth
	pShader->SetUniform("TexAmbient",1);		// FragDepth
	pShader->SetUniform("TexDiffuse",2);
	pShader->SetUniform("TexSpecular",3);

	int max_Texel_Offset = 0;
	glGetIntegerv(GL_MIN_PROGRAM_TEXEL_OFFSET,&max_Texel_Offset);
	pShader->SetUniform("InverseProjectionMatrix",SilverX::inverse(projectionMatrix));

	m_pQuad->render();



}

ArcBall* SilverScene::GetArcBall()
{
	m_pArcBall->setViewPort(m_width,m_height);
	return m_pArcBall;
}
