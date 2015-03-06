#pragma once
#include <string>
#include "../IReferenceCounted.h"
enum ETextureFiltering
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

/********************************

Class:	CTexture

Purpose:	Wraps OpenGL texture
			object and performs
			their loading.

********************************/

class CTexture : public IReferenceCounted
{
	typedef unsigned int uint;
public:
	bool loadTexture2D(std::string a_sPath, bool bGenerateMipMaps = false);
	void bindTexture(int iTextureUnit = 0);

	void setFiltering(int a_tfMagnification, int a_tfMinification);

	int getMinificationFilter();
	int getMagnificationFilter();

	void releaseTexture();

	CTexture();
private:
	int iWidth, iHeight, iBPP; // Texture width, height, and bytes per pixel
	uint uiTexture; // Texture name
	uint uiSampler; // Sampler name
	bool bMipMapsGenerated;

	int tfMinification, tfMagnification;

	std::string sPath;
};