#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>
#include "sinc.h"
#include "RegularPolygon.h"
#include "Garland.h"
#include "koch.h"
#include "Arc.h"
#include "LogicDiagram.h"
#include "Cohen_Sutherland.h"
float dx,dy;
/*----------RenderScene---------------------*/
void Layout(int n, int m, float Width, float Height)
{
	for(int i = 0; i<n; i++)
		for(int j = 0; j<n; j++)
		{
			setViewport(i*Width,(i+1)*Width,j*Height,(j+1)*Height);
			//render layout function
			//Tiling();//Render the tiling sequence
			Truchlet();
		}
}
void renderScene(void)
{
	static int i = 0;
	glClear(GL_COLOR_BUFFER_BIT); 
	glColor3f(0.0,0.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setWindow(-1+dx,1+dx,-1+dy,1+dy);
	setViewport(0,100,0,100);
	renderPointList();

	
	setWindow(0,1,0,1);
	setViewport(100,200,0,100);
	renderLogicDiagram(0.1,0.7);

	setWindow(0.0,4.0,0.0,4.0);
	setViewport(200,300,0,100);
	renderClip();
	glColor3f(0.0,0.0,1.0);
	
	setWindow(-1,1,-1,1);
	setViewport(300,400,0,100); 
	renderArc1(0.4,0.4);

	setViewport(400,500,0,100);
	renderArc2(0.4,0.4);

	setViewport(500,600,0,100);
	renderArc3(0.4,0.4,0.8);

	setWindow(-5.0,5.0,-5.0,5.0);
	setViewport(0,100,100,200);
	ElectrostaticField(2.0,2.0,1.0);

	setWindow(-5.0,5.0,-5.0,5.0);
	setViewport(0,100,200,300);
	Smith();

	setWindow(-0.5,0.5,-0.5,0.5);
	Layout(50,50,10,10);
	glFlush();
	
}

void Init(void)
{
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0,0.0,1.0);
	glLineWidth(2.0);
}

void keyboard(unsigned char theKey, int mouseX, int mouseY)
{
	switch(theKey)
	{
	case 'w':
		dy -= 0.1;
		break;
	case 's':
		dy += 0.1;
		break;
	case 'a':
		dx += 0.1;
		break;
	case 'd':
		dx -= 0.1;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("The Famous Sinc Function");
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keyboard);
	//glutIdleFunc(renderScene);
	Init();
	glutMainLoop();
}