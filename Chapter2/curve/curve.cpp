#include <windows.h> 
#include <math.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glut.h>

#pragma comment(lib,"glut.lib")

const int screenWidth = 640;
const int screenHeight = 480;
GLdouble	A,B,C,D;//propotion and transform

void sierpinski_render(void);
void coordinate();
void screen();
void myInit(void)
{
	glClearColor(1.0,1.0,1.0,0.0); //set background to white
	glColor3f(0.0f,0.0f,0.0f);//use black to paint
	glPointSize(2.0);//set the size of the point to 2x2 px

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLdouble)screenWidth,0.0,(GLdouble)screenHeight);
	A = screenWidth /4.0;
	B = 0.0;
	C = D = screenHeight/2.0;
}
void Curve(void)
{
	glBegin(GL_POINTS);
	for(GLdouble x = 0;x<4.0;x+=0.005)
	{
		GLdouble func = exp(-x) * cos (2*3.14159265 * x);
		glVertex2d(A * x + B, C * func + D);
	}
	glEnd();
}
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//sierpinski_render();
	coordinate();
	screen();
	glFlush();
}

void main(int argc, char ** argv)
{
	glutInit(&argc,argv);  //Initial the tool kit
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);//set display mode
	glutInitWindowSize(screenWidth,screenHeight);//set the size of the window
	glutInitWindowPosition(100,150);
	glutCreateWindow("Dot Plot of a Function");  //create the window
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();

}

//sierpinski_render
struct GLintPoint
{
	GLint	x;
	GLint	y;
};
void drawDot(int x,int y)
{
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();
}
void drawLine(GLintPoint a,GLintPoint b)
{
	glBegin(GL_LINES);
		glVertex2i(a.x,a.y);
		glVertex2i(b.x,b.y);
	glEnd();
}
void sierpinski_render(void)
{
	GLintPoint T[3] = {{10,10},{600,10},{300,480}};
	int index  = rand()%3;
	GLintPoint point = T[index];
	drawDot(point.x,point.y);
	drawDot(400,400);
	for(int i = 0;i<55000;i++)
	{
		index  = rand()%3;
		point.x =(point.x + T[index].x)/2;
		point.y = (point.y + T[index].y)/2;
		drawDot(point.x,point.y);
	}
	//Curve();
}
void coordinate()
{
	GLintPoint a,b;
	a.x = screenWidth/2;
	a.y = 0;
	b.x = screenWidth/2;
	b.y = screenHeight;
	drawLine(a,b);
	a.x = 0;
	a.y = screenHeight/2;
	b.x = screenWidth;
	b.y = screenHeight/2;
	drawLine(a,b);
}

void screen()
{
	for(int i = 0;i<screenWidth;i+=5)
		for(int j = 0;j<screenHeight;j+=5)
			drawDot(i,j);
}