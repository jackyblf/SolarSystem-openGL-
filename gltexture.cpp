#include "gltexture.h"
#include "tga.h"
#include "lodepng.h"
#include <vector>
#include <iostream>

glTexture::glTexture(const char *fname,
	bool make_mipmaps)
{

	std::vector<unsigned char> image;
	unsigned int width, height;
	unsigned int error = lodepng::decode(image, width, height, fname);

	if (error != 0)
	{
		std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
		return;
	}

	m_tex = 0;
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Get a texture space and make it active
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	//Set default(and fastest) texture propoties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Determine if mipmaps are used
	if (make_mipmaps)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	}
	else
	{
	
		double xPow2, yPow2;
		int ixPow2, iyPow2;
		int xSize2, ySize2;
		unsigned char* pData = NULL;

		GLint glMaxTexDim;
		//Get the maximum texture size
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);
		//Get the powers of 2 that correspond to the width and height of the original
		//or of the maxmaximum texture size if widthor height is larger than the maxmaximum texture size
		if (width <= glMaxTexDim)
			xPow2 = log((double)width) / log((double)2.0);
		else
			xPow2 = log((double)glMaxTexDim) / log((double)2.0);

		if (height <= glMaxTexDim)
			yPow2 = log((double)height) / log((double)2.0);
		else
			yPow2 = log((double)glMaxTexDim) / log((double)2.0);

		//round the power of 2 up to the nearest power of 2
		ixPow2 = (int)xPow2;
		iyPow2 = (int)yPow2;

		if (xPow2 != (double)ixPow2)
			ixPow2++;
		if (yPow2 != (double)iyPow2)
			iyPow2++;
		//convert power to actual value
		xSize2 = 1 << ixPow2;
		ySize2 = 1 << iyPow2;
		//if the new sizes are different than the old ones
		//resize and scale the "RGBAImage"
		if (xSize2 != width || ySize2 != height)
		{
			pData = (unsigned char*)malloc(xSize2 * ySize2 * 4 * sizeof(unsigned char));

			if (!pData)
				return;

			gluScaleImage(GL_RGBA,
				width,
				height,
				GL_UNSIGNED_BYTE,
				&image[0],
				xSize2,
				ySize2,
				GL_UNSIGNED_BYTE,
				pData);

			width = xSize2;
			height = ySize2;
		}
		
		glTexImage2D(GL_TEXTURE_2D,
			0, 4, width,height,
			0, GL_RGB, GL_UNSIGNED_BYTE,pData);

		if (pData)
		{
			free(pData);
			pData = NULL;
		}
	}	
}


void glTexture::MakeCurrent()
{
	glBindTexture(GL_TEXTURE_2D, m_tex);
}

glTexture::~glTexture()
{
	glDeleteTextures(1, &m_tex);
}

