#ifndef GLUTILS_H
#define GLUTILS_H
#include <string>
#include <FreeImage.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
class GLUtils
{
public:
    GLUtils();

    static int checkForOpenGLError(const char *, int);
    static void dumpGLInfo(bool dumpExtensions = false);

	//Don't use the following one function. It's messy.
	static unsigned char* readImageData(std::string path, unsigned int* imgWidth = NULL, unsigned int* imgHeight = NULL, unsigned int* imgBBP = NULL);
	
	static FIBITMAP* loadImage(std::string path);
	static glm::vec3 getArcBallVector(int x,int y,int width,int height);
	static glm::mat4 getRotMatrix(int x,int y,int width,int height);
	static glm::mat4 getIncrementalRotMatrix(glm::mat4 originMat,int x, int y, int width, int height);
};

#endif // GLUTILS_H
