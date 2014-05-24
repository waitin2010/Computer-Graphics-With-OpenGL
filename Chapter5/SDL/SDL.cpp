#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "SDL_arch.h"

Scene scn;

void render()
{
	static float angle = 0.0;
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0,0.0,0.0);
	scn.drawSceneOpenGL();
	glutSwapBuffers();	
};


void main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("Shaded example-3D scene");

	glutDisplayFunc(render);
	glutIdleFunc(render);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glClearColor(1.0,1.0,1.0,0.0);
	
	scn.read("HLSCENE1.DAT");
	glEnable(GL_LIGHTING);
	scn.makeLightsOpenGL();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double winHt = 1.0;
	glOrtho(-winHt * 64 / 48.0,winHt * 64 /48.0,-winHt,winHt,0.1,1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,50,900,0,0,0,0,1.0,0.0);
	glViewport(0,0,480,480);
	glutMainLoop();
}












