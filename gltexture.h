#ifndef GLWARP_H
#define GLWARP_H
#include "base.h"

class glTexture
{
public:
  GLuint m_tex;
	glTexture(){m_tex=0;}

	glTexture(const char* fname,
		bool make_mipmaps=true);

	~glTexture();
	
	void MakeCurrent();
};


#endif