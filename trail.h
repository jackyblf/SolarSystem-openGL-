#ifndef TRAIL_H
#define TRAIL_H
#include "base.h"
#include <vector>
#include "glTexture.h"
class glTrail
{
public:
	std::vector<vec3> path;
	float width;
	float color[4];
	glTexture* texture;

	glTrail(const char* texname) :
		width(0.2)
	{
		color[0] = 1.0f;
		color[1] = 1.0f;
		color[2] = 1.0f;
		color[3] = 1.0f;
		texture = new glTexture(texname, true);
	}

	virtual ~glTrail()
	{
		delete texture;
	}

	//设置trail的位置坐标
	void setPath(vec3 pos)
	{
		for (int i = 0; i < 5; i++)
			path.push_back(vec3((i + 0.5f), 0, 0));
	}

	void draw(vec3 pos)
	{
		vec3 v, v1, v2, v3, z;
		float f;
		int i;

		if (path.size() <= 1)
			return;

		texture->MakeCurrent();

		//深度写入禁止，但是深度比较还是需要的啦
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBegin(GL_QUAD_STRIP);

		for (i = 0; i < path.size(); i++)
		{
			z = pos - path[i];
			v3.x = v3.y = v3.z = 0.0f;
			if (i > 0)
			{
				v1 = path[i] - path[i - 1];
				v2.cross(z, v1);
				v2.normalize();
				v3 += v2;
			}
			if (i < (path.size() - 1))
			{
				v1 = path[i + 1] - path[i];
				v2.cross(z, v1);
				v2.normalize();
				v3 += v2;
			}
			v3.normalize();

			f = (float)i / (path.size() - 1);
			v = path[i] + v3*width;
			glTexCoord2f(0, f);
			glVertex3fv(&v.x);
			v = path[i] - v3*width;
			glTexCoord2f(1, f);
			glVertex3fv(&v.x);
		}
		glEnd();
		glDepthMask(GL_FALSE);
		glDisable(GL_BLEND);
	}
};
#endif