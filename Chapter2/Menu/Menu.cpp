
/*Menu Design and use
	1.create:glutCreateMenu()
	2.Adding menu item:glutAddMenuEntry()
	3.binging the menu to the mouse:glutattachmenu()
	4.function for menu
*/
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define RED	1
#define GREEN	2
#define	BLUE	3
#define	WHITE	4

float angle = 0.0;
float red = 1.0,blue=1.0,green = 1.0;

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(angle,0.0,1.0,0.0);
	glColor3f(red,green,blue);
	glBegin(GL_TRIANGLES);
		glVertex3f(-0.5,-0.5,0.0);
		glVertex3f(0.5,0.0,0.0);
		glVertex3f(0.0,0.5,0.0);
	glEnd();
	angle++;
	glutSwapBuffers();

}

void processMenuEvents(int option)
{
	switch(option)
	{
	case RED:
		red = 1.0;green = 0.0;blue = 0.0;break;
	case BLUE:
		red = 0.0;green = 0.0;blue = 1.0;break;
	case GREEN:
		red = 0.0;green = 1.0;blue = 0.0;break;
	case WHITE:
		red = 1.0;green = 1.0;blue = 1.0;break;
	default:
		break;
	}
}

void main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(320,320);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Menu Test");
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Red",RED);
	glutAddMenuEntry("Blue",BLUE);
	glutAddMenuEntry("Green",GREEN);
	glutAddMenuEntry("White",WHITE);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
}