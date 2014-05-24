#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

const int num = 500 ;
GLint A = 1103515245, B = 12345, N = 32767;
GLint screenWidth = 100,screenHeight = 100;
GLdouble a,b,c,d;
void Init(void)
{
	glClearColor(1.0,1.0,1.0,0.0);  //set background to white
	glColor3f(0.0,0.0,0.0);  //set black as painted color
	glPointSize(2.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLdouble)screenWidth,0.0,(GLdouble)screenHeight);
	a = screenWidth/N;
	b = 0.0;
	c = d = screenHeight/N;

}
void renderScene(void)
{
	static int ni = 20;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_POINTS);
		for(int i =0;i<num;i++)
		{
			int   func = (ni *( A % N) + B%N ) % N;
			glVertex2i(rand()%screenWidth, rand() % screenHeight);
			ni = func;
		}
		
	glEnd();
	glFlush();

}



void main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(screenWidth,screenHeight);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Pseudo_random point cloud");
	glutDisplayFunc(renderScene);
	Init();
	glutMainLoop();
}