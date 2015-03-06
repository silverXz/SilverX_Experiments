#include "glutils.h"

#include "cookbookogl.h"

#include <cstdio>




GLUtils::GLUtils() {}

int GLUtils::checkForOpenGLError(const char * file, int line) {
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
		const char * message = "";
		switch( glErr )
		{
		case GL_INVALID_ENUM:
			message = "Invalid enum";
			break;
		case GL_INVALID_VALUE:
			message = "Invalid value";
			break;
		case GL_INVALID_OPERATION:
			message = "Invalid operation";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			message = "Invalid framebuffer operation";
			break;
		case GL_OUT_OF_MEMORY:
			message = "Out of memory";
			break;
		default:
			message = "Unknown error";
		}

        printf("glError in file %s @ line %d: %s\n", file, line, message);
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;

}

void GLUtils::dumpGLInfo(bool dumpExtensions) {
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    printf("GL Vendor    : %s\n", vendor);
    printf("GL Renderer  : %s\n", renderer);
    printf("GL Version   : %s\n", version);
    printf("GL Version   : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);

    if( dumpExtensions ) {
        GLint nExtensions;
        glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
        for( int i = 0; i < nExtensions; i++ ) {
            printf("%s\n", glGetStringi(GL_EXTENSIONS, i));
        }
    }
}

unsigned char* GLUtils::readImageData(std::string path, unsigned int* imgWidth, unsigned int* imgHeight, unsigned int* imgBBP)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(path.c_str(),0);

	if( fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(path.c_str());
	
	if( fif == FIF_UNKNOWN)
		return NULL;

	if( FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif,path.c_str());

	if(!dib)
		return NULL;

	unsigned char* imgData = FreeImage_GetBits(dib);
	if(imgWidth != NULL)
		*imgWidth = FreeImage_GetWidth(dib);
	if(imgHeight != NULL)
		*imgHeight = FreeImage_GetHeight(dib);
	if(imgBBP != NULL)
		*imgBBP = FreeImage_GetBPP(dib);
	return imgData;
}

FIBITMAP* GLUtils::loadImage(std::string path)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(path.c_str(),0);

	if( fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(path.c_str());

	if( fif == FIF_UNKNOWN)
		return NULL;

	if( FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif,path.c_str());

	if(!dib)
		return NULL;
	return dib;
}

//This function tends to get the arcball vector corresponding to a point position in the Screen Coordinate.
//It's slightly different from the OpScene get_arcball_vector(). The OpScene One calculate the vector pointing out the screen while this one
// pointing into the screen. Because we are trying to move the viewer's perspective instead of rotating an object.
//param: int x  X-coordinate of the mouse's current position.
//param: int y  Y-coordinate of the mouse's current position.
//param: int width  Valid Width Of The Client Rect.
//param: int height  Valid Height Of The Client Rect.
glm::vec3 GLUtils::getArcBallVector(int x,int y,int width,int height)
{
	float rectWidth = width;
	float rectHeight = height;
	glm::vec3 P = glm::vec3(2.0*(float)x / rectWidth - 1.0, 2.0*(float)y /rectHeight - 1.0 , 0.0);
	P.y = - P.y;
	float OP_squared = P.x*P.x + P.y * P.y;
	if(OP_squared <= 1.0)
		P.z = sqrt(1.0f - OP_squared);
	else
		P = glm::normalize(P);
	P.z = -P.z;			//This is the mainly difference from the OpScene One. 
	return P;
}

glm::mat4 GLUtils::getRotMatrix(int x,int y,int width,int height)
{
	glm::vec3 vb = getArcBallVector(x,y,width,height);
	glm::vec3 va = getArcBallVector(width/2,height/2,width,height);
	float rotateRadius = 1.5* acos(glm::min(1.0f,glm::dot(va,vb)));
	glm::vec3 axis_in_camera_coord = glm::cross(va,vb);
	glm::mat4 retMatrix = glm::rotate(glm::mat4(1.0),glm::degrees(rotateRadius),axis_in_camera_coord);
	return retMatrix;
}

glm::mat4 GLUtils::getIncrementalRotMatrix(glm::mat4 originMat,int x, int y, int width, int height)
{
	glm::vec3 vb = getArcBallVector(x,y,width,height);
	glm::vec3 va = getArcBallVector(width/2,height/2,width,height);
	float rotateRadius = 1.5* acos(glm::min(1.0f,glm::dot(va,vb)));
	glm::vec3 axis_in_camera_coord = glm::cross(va,vb);
	glm::mat4 retMatrix = glm::rotate(originMat,glm::degrees(rotateRadius),axis_in_camera_coord);
	return retMatrix;
}