#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <stdio.h>

GLint	M = 40,
		L = 3;
const GLint screenWidth = 640,
			screenHeight = 480;			//the width and height of the windows
struct GLintPoint {
	int x;
	int y;
};
GLintPoint point = {0,0};
GLintPoint points[1000];
GLint index = 0;

void Init()
{
	glClearColor(1.0,1.0,1.0,0.0);		//Set the background to white
	glColor3f(0.0,0.0,0.0);				//Set the painted color to black
	glPointSize(2.0);					//Set the size of the points to 2.0
	glMatrixMode(GL_PROJECTION);		//choose the projection mode to matrix
	glLoadIdentity();					//Load indentity
	gluOrtho2D(0.0,(GLdouble)screenWidth,0.0,(GLdouble)screenHeight);
}

void RenderScene()
{
	int A = M * (1 + 2 * L );
	int B = L * M;
	GLintPoint temp={0,0};
	glClear(GL_COLOR_BUFFER_BIT);		//Clear the screen
	glBegin(GL_POINTS);
		for(int i = 0;i<index;i++)
		{
			point.x = points[i].x;
			point.y = points[i].y;
			while(1){
			glVertex2i(point.x,point.y);
			point.x = A - point.y + abs(point.x - B );
			point.y = point.x;
			if(temp.x == point.x && temp.y == point.y)
				break;
			else
			{
				temp.x = point.x;
				temp.y = point.y;
			}
			}
		}
	glEnd();
	glFlush();
}
void myMouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		points[index].x = x;
		points[index++].y = screenHeight - y;
		RenderScene();
		glFlush();
	}

}
void main(int argc,char**argv)
{
	glutInit(&argc,argv);				//Init the tool kit
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);	//Init the display mode
	glutInitWindowPosition(100,100);			//Init the position and size of the window
	glutInitWindowSize(screenWidth,screenHeight);
	glutCreateWindow("Gingerbread Man");
	glutDisplayFunc(RenderScene);
	glutMouseFunc(myMouse);
	Init();
	glutMainLoop();

}