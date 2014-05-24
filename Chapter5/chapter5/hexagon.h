#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

void hexagon(float radius)
{
	glBegin(GL_LINE_LOOP);
	   for(int i =0; i < 6; i++)
		   glVertex2f(radius * cos(M_PI/3 * i),radius * sin(M_PI/3 * i));
	glEnd();
}