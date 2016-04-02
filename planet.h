#ifndef PLANET_H
#define PLANET_H
#include "vec3.h"
#include "gltexture.h"
class planet
{
public:
  float aroundRotatedSpeed;//公转
  float selfRotatedSpeed;//自转
  float radius;//行星的半径，影响绘制的大小啦
  //下面这个变量代表当前行星离父亲之间的的距离拉
  //1.太阳系以太阳为中心进行运动
  //2.除地球外其他行星相对于太阳进行自转和公转，因此对于其他行星来说，太阳就是父亲，其他行星就是儿子
  //3比较特殊的是地球，地球也是相对太阳进行自转和公转的，但是地球还有一个儿子，既月球，因此对于月球来说，他的父亲是地球，他的父亲的父亲是太阳
  //层次关系总结如下：
  //   太阳
  //      除地球和月球外的其他行星
  //      地球
  //         月球
  vec3 pos;
 
  glTexture *texture;
public:
 
  planet(const char* texname,float as,float ss,float radius,vec3 pos)
  {
    texture=new glTexture(texname,true);
    this->aroundRotatedSpeed =as;
    this->selfRotatedSpeed =ss;
    this->radius =radius;
    this->pos =pos;
   
  }

  ~planet()
  {
	  if (!texture)
	  {
		  delete texture;
		  texture = NULL;
	  }
  }

};
#endif