#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gl/glut.h>
#include <stdio.h>

const GLint screenWidth = 640,screenHeight = 320;
GLdouble A,B,C,D;
GLint y0;
unsigned long	 y,k,ymax;
void Hail();
void Init(void)
{
	Hail();
	glClearColor(1.0,1.0,1.0,0.0);  //set background to white
	glColor3f(0.0,0.0,0.0);  //set black as painted color
	glPointSize(4.0);        //set the size of the points
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,screenWidth,0.0,screenHeight);

}
void Hail()							/*calculate the bigest of y and the biggest of the k*/
{
	k = 0;
	scanf("%ld",&y0);
	y = y0;
	ymax = y;
	while(y>1)
	{
		y = y % 2? (3*y+1):y/2;
		ymax = ymax >= y? ymax: y;
		k++;
	}
	C = screenHeight / ymax;
	A = screenWidth/k;
	B = D = 0;

}
void RenderScene(void)
{
	unsigned long y = y0,k=0;
   glClear(GL_COLOR_BUFFER_BIT);
   glBegin(GL_POINTS);
		do
		{
			
			glVertex2f(A * k + B, (double)y / ymax * (screenHeight-100) + D);	//draw the point(k,y)
			y = y %2 ? ( 3* y + 1): y/2;										// calculate the next point
			k++;
		}while(y>1);
	glEnd();
	glFlush();

}

void main(int argc,char** argv)
{
	glutInit(&argc,argv);                      //init the tool kit
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); //setup the displaymode
	glutInitWindowPosition(100,100);		   //setup the window position and size
	glutInitWindowSize(screenWidth,screenHeight);
	glutCreateWindow("Hail sequence");		   //create the window
	glutDisplayFunc(RenderScene);			  // register the callback function for the display
	Init();
	glutMainLoop();
}