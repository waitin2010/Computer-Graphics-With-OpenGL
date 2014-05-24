#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>

GLint screenWidth = 600, screenHeight = 800;
struct GLintPoint{
	int x;
	int y;
};
void Init(void)
{
	glClearColor(1.0,1.0,1.0,0.0);		//Set the background to white
	glColor3f(0.0,0.0,0.0);				//Set the painted color to black
	glPointSize(2.0);					//Set the size of the points to 4.0
	glMatrixMode(GL_PROJECTION);		//Use the GL_PROJECTION Mode
	glLoadIdentity();					//Load Indentity
	gluOrtho2D(0.0,screenWidth,0.0,screenHeight);		//
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);		//clear the screen
	GLintPoint	T[3] = {
		{10,100},
		{600,100},
		{300,600}
	};								// the three points of some triangle
	int index = rand()%3;
	GLintPoint point = T[index];
	glBegin(GL_POINTS);
		for(int i = 0; i < 20000; i++)
		{
				glVertex2i(point.x,point.y);
				index = rand()%3;
				point.x = (point.x + T[index].x)/2;
				point.y = (point.y + T[index].y)/2;
		}
		glVertex2i(point.x,point.y);
	glEnd();
	glFlush();

}

void main(int argc, char** argv)
{
	glutInit(&argc,argv);			//Init the tool kit
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);		//Init the display mode
	glutInitWindowSize(screenWidth,screenHeight);
	glutInitWindowPosition(100,100);				//Init the window position and size
	
	glutCreateWindow("OpenGL makes this all so easy!");
	glutDisplayFunc(RenderScene);                  //Register the callback function for display
	Init();
	glutMainLoop();
}