#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Mesh.h"
#pragma comment(lib,"glut32.lib")

GLint screenWidth = 640,screenHeight = 480;
Mesh house;

void Init(void)
{
	glClearColor(1.0,1.0,1.0,0.0);			//set background to white
	glColor3f(0.0,0.0,0.0);					//Set Black as the paint color

	//Init meshobject
	house.readFile("house.txt");
}

void setWindow(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left,right,bottom,top,0.1,100.0);
}

void setViewport(GLint left, GLint  right, GLint bottom, GLint top)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(left,bottom,right - left, top - bottom);
	//gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);
}

void renderScene(void)
{
	static float angle = 0.0;
	glClear(GL_COLOR_BUFFER_BIT);
	setWindow(-3.0,3.0,-3.0,3.0);
	setViewport(0,screenWidth,0,screenHeight);
	glTranslated(0.0,0.0,-2.0);
	glRotated(angle,0.0,1.0,0.0);
	house.draw();
	angle+= 0.1;
	glFlush();
}

void main(int argc, char ** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(screenWidth,screenHeight);
	glutCreateWindow("chapter 5");
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	Init();
	glutMainLoop();
}