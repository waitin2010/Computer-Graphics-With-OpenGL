#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

const GLint screenWidth = 600,
			screenHeight = 400;
GLdouble theta = (1 + sqrt((double)5))/ 2;
GLint temp = (screenHeight - screenWidth / theta)/2;
GLint L = 0,
	  R = 600,
	  T = 385,
	  D = 15;

void Init(void)
{
	glClearColor(1.0,1.0,1.0,0.0);			//set the background to white
	glColor3f(0.0,0.0,0.0);					//set the painted color to black
	glMatrixMode(GL_PROJECTION);			//Choose the projection mode
	glLoadIdentity();						//Load the Indentity
	gluOrtho2D(0.0,(GLdouble)screenWidth,0.0,(GLdouble)screenHeight);

}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);			//Clear screen
	glBegin(GL_LINE_STRIP);
		glVertex2i(L,D);
		glVertex2i(L,T);
		glVertex2i(R,T);
		glVertex2i(R,D);
		glVertex2i(L,D);
	glEnd();
	glBegin(GL_LINES);
		while(1)
		{
			L = L + T - D;		//Calculate the new value of L
			if( L >= R)
				break;
			glVertex2i(L,D);
			glVertex2i(L,T);

			T = T - ( R - L );
			if(T <= D)
				break;
			glVertex2i(L,T);
			glVertex2i(R,T);

			R = R - ( T - D );
			if( L >= R )
				break;
			glVertex2i(R,T);
			glVertex2i(R,D);

			D = D + ( R - L);
			if(T <= D)
				break;
			glVertex2i(L,D);
			glVertex2i(R,D);


		}
	glEnd();
	glFlush();
}

void main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(screenWidth,screenHeight);
	glutCreateWindow("Golden Section");
	glutDisplayFunc(RenderScene);
	Init();
	glutMainLoop();
}