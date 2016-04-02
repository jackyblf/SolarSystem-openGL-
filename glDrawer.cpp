#include "glDrawer.h"
GLUquadricObj * m_quad = NULL;

void drawInit()
{
  m_quad = gluNewQuadric();
}

void drawDeInit()
{
  gluDeleteQuadric(m_quad);
}

void drawSphere(double radius , int slices, int stack ,bool texture)
{  
  
  gluQuadricTexture(m_quad, true);
  gluSphere(m_quad,radius,slices,stack);
}

void drawDisk(GLdouble            innerRadius, 
    GLdouble            outerRadius, 
    GLint               slices )
{
  gluDisk(m_quad,innerRadius, 
    outerRadius, 
    slices, 
    true);
}