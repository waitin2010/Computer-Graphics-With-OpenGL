#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gl/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include "hexagon.h"
#pragma comment(lib,"glut.lib")

void SetWindow(GLdouble left, GLdouble right, GLdouble bottom,GLdouble top)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left,right,bottom,top);
}

void SetViewport(GLint left, GLint right, GLint bottom, GLint top)
{
	glViewport(left,bottom,right - left, top - bottom);
}

void initCT(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void scale2D( double sx, double sy)
{
	glMatrixMode(GL_MODELVIEW);
	glScaled(sx,sy,1.0);
}

void translate2D(double dx, double dy)
{
	glMatrixMode(GL_MODELVIEW);
	glTranslated(dx,dy,0.0);
}

void rotate2D(double angle)
{
	glMatrixMode(GL_MODELVIEW);
	glRotated(angle,0.0,0.0,1.0);
}

void pushCT(void)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}
void checkStack(void)
{
	
}
void popCT()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void Init()
{
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0,0.0,0.0);

}

void myMouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN){
		if(button == GLUT_LEFT_BUTTON)
		{
			
			glutPostRedisplay();
	     }
	}
}
void RenderDynamic()
{
	static float t = 0.0;
	SetWindow(-1.0,1.0,-1.0,1.0);
	SetViewport(0,480,0,480);

}
void RenderStatic()
{
	SetWindow(-1.0,1.0,-1.0,1.0);
	SetViewport(0,480,0,480);
	glBegin(GL_LINE_LOOP);
		glVertex2f(0.0,0.0);
		glVertex2f(0.0,0.1);
		glVertex2f(0.1,0.0);
	glEnd();
}
void collageHexagon(int n, int m, float r, float v)
{
	for(int i = 0; i < n;i++)
	{
		pushCT();
		for(int j=0; j <ceil((float) m/2);j++)
		{
			hexagon(r);
			translate2D(3*r,0);
		}
		popCT();
		if(i%2==0)
			translate2D(3*r/2,v);
		else
			translate2D(-3*r/2,v);
	}
}
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	
	pushCT();
	SetWindow(-10.0,10.0,-10.0,10.0);
	SetViewport(0,480,0,480);
	collageHexagon(6,9,0.5,0.5*cos(M_PI/6));
	popCT();
	
	glutSwapBuffers();

}

void main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("Chapter 5 Solution");
	glutDisplayFunc(Render);
	//glutIdleFunc(Render);//use for animation
	glutMouseFunc(myMouse);
	Init();
	glutMainLoop();
}