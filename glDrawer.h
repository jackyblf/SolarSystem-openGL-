#ifndef GLDRAWER_H
#define GLDRAWER_H
#include "base.h"


void drawInit();
void drawSphere(double radius , int slices, int stack,bool texture);
void drawDisk( GLdouble            innerRadius, 
               GLdouble            outerRadius, 
               GLint               slices);
void drawDeInit();
#endif