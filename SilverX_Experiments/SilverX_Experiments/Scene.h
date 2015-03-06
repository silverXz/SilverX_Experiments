#ifndef _SILVERX_SCENE_H_
#define _SILVERX_SCENE_H_

#include <glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

class WxCamera;
class CxMesh;
class ShaderLib;
class Quad;

struct CursorPos
{
	int x;
	int y;
	CursorPos():x(0),y(0) {}
	bool operator==(const CursorPos& cp)
	{
		return (x == cp.x && y == cp.y);
	}
};



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

	void Pinch(int x, int y);
	void Move(int x,int y);
	void Release(int x,int y);

	void SetupFramebuffer();

	void ZoomIn();

	void ZoomOut();

	void LoadMesh(const char* filePath);

	void Resize(int w,int h);

	glm::vec3 Get_Arcball_Vector(int,int);


private:
	int m_width;
	int m_height;

	ShaderLib* m_pShaderLib;

	WxCamera* m_pCamera;

	CxMesh* m_pMesh;

	Quad* m_pQuad;

	GLuint m_hFBO;
	GLuint m_hAO;


	CursorPos last,cur;

	bool m_bPinched;

	bool m_bSceneInited;

	bool m_bFBOInited;
};


#endif