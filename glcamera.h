#ifndef CAMERA_H
#define CAMERA_H
#include "base.h"
struct glCamera
{
  vec3  pos;//摄像机的世界空间坐标
  float viewMatrix[16];
  vec3  forward;
  vec3  right;
  vec3  up;

public :

  glCamera( vec3 at)
  {
    pos=at;
  }

  void getViewMatrix()
  {
    glMatrixMode(GL_MODELVIEW);
    glGetFloatv(GL_MODELVIEW_MATRIX,viewMatrix);
  }

  void update()
  {
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glTranslatef(-pos.x,-pos.y,-pos.z);

   getViewMatrix();

   right=vec3(viewMatrix[0],viewMatrix[4],viewMatrix[8]);
   up=vec3(viewMatrix[1],viewMatrix[5],viewMatrix[9]);
   forward=vec3(viewMatrix[2],viewMatrix[6],viewMatrix[10]);
  }
      
};
#endif