#include "Texture.h"

#include <gl/glew.h>


#include <FreeImage.h>

#pragma comment(lib,"FreeImage.lib")

CTexture::CTexture()
{
	bMipMapsGenerated = false;
}


bool CTexture::loadTexture2D(std::string a_sPath,bool bGenerateMipMaps)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	//freeimage bitmap pointer
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(a_sPath.c_str(),0);

	if(fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(a_sPath.c_str());
	
	//Unknown picture format!
	if(fif == FIF_UNKNOWN)
		return false;

	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif,a_sPath.c_str());  //This is the main import/read operation!

	if(!dib)
		return false;

	BYTE* bDataPtr = FreeImage_GetBits(dib);
	iWidth = FreeImage_GetWidth(dib);
	iHeight = FreeImage_GetHeight(dib);
	iBPP = FreeImage_GetBPP(dib);

	//!if somehow one of these failed (they shouldn't), return failure
	
	if(bDataPtr == NULL || iWidth == 0 || iHeight == 0)
		return false;

	//!Generate Texture Object in OpenGL
	//On most occasions, glGenXXXX is followed by glBindXXXX, keep that in mind!
	glGenTextures(1,&uiTexture);
	glBindTexture(GL_TEXTURE_2D,uiTexture);


	//?What's these for?
	// if bit per pixel is 24, then the format is GL_RGB, no alpha.
	//if not 24. if is 8. then the format is GL_LUMINANCE. if not 8, then 0;
	int iFormat = iBPP == 24 ? GL_BGR : iBPP == 8 ? GL_LUMINANCE : 0;
	int iInternalFormat = iBPP == 24 ? GL_RGB :GL_DEPTH_COMPONENT;

	//Translate the picture data to GPU I think.
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,iWidth,iHeight,0,iFormat,GL_UNSIGNED_BYTE,bDataPtr);

	if(bGenerateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(dib);

	glGenSamplers(1,&uiSampler);

	sPath = a_sPath;
	bMipMapsGenerated = bGenerateMipMaps;
	
	return true;
}

void CTexture::setFiltering(int a_tfMagnification, int a_tfMinification)
{
	// Set magnification filter
	if(a_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if(a_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	tfMinification = a_tfMinification;
	tfMagnification = a_tfMagnification;
}

void CTexture::bindTexture(int iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glBindSampler(iTextureUnit, uiSampler);
}

void CTexture::releaseTexture()
{
	glDeleteSamplers(1, &uiSampler);
	glDeleteTextures(1, &uiTexture);
}

int CTexture::getMinificationFilter()
{
	return tfMinification;
}

int CTexture::getMagnificationFilter()
{
	return tfMagnification;
}