#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>

#pragma once
const float pi = 3.14159265358979;
/*------------SetWindow----------------------*/
void setWindow(GLdouble left,GLdouble right, GLdouble buttom, GLdouble top)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left,right,buttom,top);
}

/*-----------SetViewport--------------------*/
void setViewport(GLint left, GLint right, GLint buttom, GLint top)
{
	glViewport(left,buttom,right - left, top - buttom);
}
