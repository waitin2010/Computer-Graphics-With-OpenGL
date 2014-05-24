#include <windows.h> 
#include <math.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glut.h>

#pragma comment(lib,"glut.lib")

const int screenWidth = 640;
const int screenHeight = 480;
bool selected = false;

struct GlintPoint
{
	GLint x,y;
};
GlintPoint corner[2];


void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1.0f,1.0f,0.0f);
	if(selected)
	{
		glBegin(GL_QUADS);
		glVertex2i(corner[0].x,corner[0].y);
		glVertex2i(corner[0].x,corner[1].y);
		glVertex2i(corner[1].x,corner[1].y);
		glVertex2i(corner[1].x,corner[0].y);
		glEnd();
	}
	glBegin(GL_LINES);
	glVertex2i(0,100);
	glVertex2i(100,100);
	glVertex2i(100,300);
	glVertex2i(200,300);
	glEnd();
	glutSwapBuffers();
}
void myMouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		corner[0].x = x;
		corner[0].y = screenHeight - y;
		selected = true;
		
	}
	glutPostRedisplay();
}

void myPssiveMotion( int x, int y)
{
	corner[1].x = x;
	corner[1].y = screenHeight - y;
	glutPostRedisplay();

}
void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
	GLint x = mouseX;
	GLint y = screenHeight-mouseY;
	switch(theKey)
	{
	case 'p':
		glPointSize(4.0f);
		break;
	case 'E':
		exit(-1);
		break;
	default:
		break;

	}
}
void main(int argc, char ** argv)
{
	glutInit(&argc,argv);  //Initial the tool kit
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);//set display mode
	glutInitWindowSize(screenWidth,screenHeight);//set the size of the window
	glutInitWindowPosition(100,150);
	glutCreateWindow("Rubber Rect Demo");  //create the window
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLdouble)screenWidth,0.0,(GLdouble)screenHeight);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(1.0,1.0,1.0,0.0);
	glViewport(0,0,screenWidth,screenHeight);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	/*glutPassiveMotionFunc(myPssiveMotion);*/
	glutKeyboardFunc(myKeyboard);
	
	glutMainLoop();

}

