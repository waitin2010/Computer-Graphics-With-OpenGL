#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "SDL_arch.h"
#include "camera.h"

Scene scn;
Camera cam; // make a global camera object

void Init(void)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glClearColor(1.0,1.0,1.0,0.0);

	scn.read("HLSCENE1.DAT");
	glEnable(GL_LIGHTING);
	scn.makeLightsOpenGL();
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double winHt = 1.0;
	glOrtho(-winHt * 64 / 48.0,winHt * 64 /48.0,-winHt,winHt,0.1,1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,50,900,0,0,0,0,1.0,0.0);*/
	cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0); // make the initial camera
	cam.setShape(30.0f, 64.0f/48.0f, 0.5f, 50.0f);
	glViewport(0,0,480,480);
}
void render()
{
	static float angle = 0.0;
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0,0.0,0.0);
	scn.drawSceneOpenGL();
	glutSwapBuffers();	
};


//<<<<<<<<<<<<<<<<<<<<<<<< myKeyboard >>>>>>>>>>>>>>>>>>>>>>
void myKeyboard(unsigned char key, int x, int y)
{
	switch(key)
	{	

		// controls for camera
	case 'L':      cam.slide(.2, 0, 0); break;// slide camera right
	case 'L' - 64: cam.slide(-0.2, 0, 0); break; // slide camera left

	case 'U':      cam.slide(0, .2, 0); break;// slide camera up
	case 'U' - 64: cam.slide(0, -0.2, 0); break; // slide camera down

	case 'F':    cam.slide(0,0, 0.2); break; // slide camera forward
	case 'F'-64: cam.slide(0,0,-0.2); break; //slide camera back	
		// add up/down and left/right controls	
	case 'P':      cam.pitch(-1.0); break;
	case 'P' - 64: cam.pitch( 1.0); break;
		// add yaw controls
	case 'Y':      cam.yaw(-1.0); break;
	case 'Y' - 64: cam.yaw( 1.0); break;
		// add roll controls
	case 'R':      cam.roll(1.0); break;
	case 'R' - 64: cam.roll(-1.0); break;
	}
	glutPostRedisplay(); // draws it again
}

void main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("Shaded example-3D scene");
	glutDisplayFunc(render);
	glutIdleFunc(render);
	glutKeyboardFunc(myKeyboard);
	Init();
	glutMainLoop();
}












