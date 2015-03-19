#ifndef _SILVERX_SCENE_H_
#define _SILVERX_SCENE_H_

#include <glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

class WxCamera;
class ShaderLib;
class Quad;
class ArcBall;
class Box;

class SilMesh;

class SilverScene
{
public:
	SilverScene();
	~SilverScene();




public:

	void Init();
	void RenderScene();

	void RenderSceneMRT();

	void RenderSceneSSAO();

	void SetupFramebuffer();

	void ZoomIn();

	void ZoomOut();

	void LoadMesh(const char* filePath);

	ArcBall* GetArcBall();
	
	void Resize(int w,int h);



private:
	int m_width;
	int m_height;

	ShaderLib* m_pShaderLib;

	ArcBall* m_pArcBall;

	WxCamera* m_pCamera;

	SilMesh* m_pSilMesh;

	Quad* m_pQuad;
	Box* m_pBox;

	GLuint m_hFBO;
	GLuint m_hAO;
	GLuint m_hBlur;
	GLuint m_hRandomTex;
	GLuint m_hTexNormDepth;

	glm::vec3 Samples[16];




	bool m_bSceneInited;

	bool m_bFBOInited;
};


#endif