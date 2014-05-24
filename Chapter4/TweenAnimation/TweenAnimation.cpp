#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>
#include "TweenAnimation.h"
#include "Circle.h"
#include "InOrOut.h"
#include "rayTracing.h"
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

void Init()
{
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0,0.0,0.0);
	GetPointList();

	SceneInit();
}

void myMouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN){
		if(button == GLUT_LEFT_BUTTON)
		{
			y = 480 - y;
			float xx = x/240.0 - 1;
			float yy = y/240.0 - 1;
			int i = 0;
			float len1 = (xx-Triangle[0].x)*(xx-Triangle[0].x) + (yy-Triangle[0].y)*(yy-Triangle[0].y);
			float len2 = (xx-Triangle[1].x)*(xx-Triangle[1].x) + (yy-Triangle[1].y)*(yy-Triangle[1].y);
			float len3 = (xx-Triangle[2].x)*(xx-Triangle[2].x) + (yy-Triangle[2].y)*(yy-Triangle[2].y);
			if(len1>len2)
			{
				i=1;
				if(len2>len3)
					i = 2;
			}else if(len1 > len3)
				i = 2;
			Triangle[i].x = xx;
			Triangle[i].y = yy;
			Q.set(xx,yy);//set the test point Q
			updateRay();
			glutPostRedisplay();
	     }
	}
}
void RenderDynamic()
{
	static float t = 0.0;
	SetWindow(-1.0,1.0,-1.0,1.0);
	/*SetViewport(0,100,0,100);
	RenderPoly(Tree,11);
	SetViewport(500,600,0,100);
	RenderPoly(F,11);*/
	/*for(float t = 0.0; t<= 1.0; t+= 0.1)
	{
	SetViewport(t*1000,(t+0.1)*1000,0,100);
	RenderTween(Tree,F,11,t);
	}*/
	SetViewport(0,480,0,480);
	RenderTween(Tree,F,11,t);
	static bool down = false;
	if(t>=1.0)
	{
		down = true;

	}else if(t<=0.0)
	{
		down = false;
	}
	if(down)
		t -= 0.001;
	else 
		t += 0.001;
}
void RenderStatic()
{
	SetWindow(-1.0,1.0,-1.0,1.0);
	SetViewport(0,480,0,480);
	RenderTriangle(Triangle);
	glColor3f(sin((float)rand()),cos((float)rand()),tan((float)rand()));
	GetCenterAndRadius(Triangle,center,radius);
	RenderCircle(center,radius);

	glColor3f(sin((float)rand()),cos((float)rand()),tan((float)rand()));
	GetInnerCircle(Triangle,InnerTriangle);
	GetCenterAndRadius(InnerTriangle,center,radius);
	RenderCircle(center,radius);

	glColor3f(sin((float)rand()),cos((float)rand()),tan((float)rand()));
	GetNineCircle(Triangle,NineTriangle);
	GetCenterAndRadius(NineTriangle,center,radius);
	RenderCircle(center,radius);
	RenderPoints();
}
void RenderInOrOuter()
{
	SetWindow(-2.0,2.0,-2.0,2.0);
	SetViewport(0,480,0,480);
	DrawPolygon();
	testQ();
}
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	SetWindow(-2.0,2.0,-2.0,2.0);
	SetViewport(0,480,0,480);
	DrawScene();
	updateRay();
	glutSwapBuffers();

}

void main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("Chapter 4 Solution");
	glutDisplayFunc(Render);
	glutIdleFunc(Render);//use for animation
	glutMouseFunc(myMouse);
	Init();
	glutMainLoop();
}