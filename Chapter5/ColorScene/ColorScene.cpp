#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void wall(double thickness)
{
	//draw thin wall with top = xz - plane, corner at origin
	glPushMatrix();
	glTranslated(0.5,0.5 * thickness,0.5);
	glScaled(1.0,thickness,1.0);
	glutSolidCube(1.0);
	glPopMatrix();
}

void tableLeg(double thick, double len)
{
	glPushMatrix();
	glTranslated(0,len/2,0);
	glScaled(thick,len,thick);
	glutSolidCube(1.0);
	glPopMatrix();
}

void jackPart()
{
	//draw one axis of the unit jack-a stretched sphere
	glPushMatrix();
	glScaled(0.2,0.2,1.0);
	glutSolidSphere(1,15,15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,0,1.2);
	glutSolidSphere(0.2,15,15);//ball on one end
	glTranslated(0,0,-2.4);
	glutSolidSphere(0.2,15,15);//ball on the other end
	glPopMatrix();


}

void jack()
{
	//draw a unit jack out of spheroids
	glPushMatrix();
	jackPart();
	glRotated(90.0,0,1,0);
	jackPart();
	glRotated(90.0,1.0,0,0);
	jackPart();
	glPopMatrix();
}

void table(double topWid, double topThick, double legThick, double legLen)
{
	//draw the table- a top and four legs
	glPushMatrix();
	glTranslated(0,legLen,0);
	glScaled(topWid,topThick,topWid);
	glutSolidCube(1.0);
	glPopMatrix();
	
	double dist = 0.95* topWid/2.0 - legThick /2.0;
	glPushMatrix();
	glTranslated(dist,0,dist);
	tableLeg(legThick,legLen);
	glTranslated(0,0,-2*dist);
	tableLeg(legThick,legLen);
	glTranslated(-2*dist,0,2*dist);
	tableLeg(legThick,legLen);
	glTranslated(0,0,-2*dist);
	tableLeg(legThick,legLen);
	glPopMatrix();
}


void displaySolid(void)
{
	//set properties of the surface material
	GLfloat mat_ambient[] = {0.7f,0.7f,0.7f,1.0f};  //gray
	GLfloat mat_diffuse[] = {0.6f,0.6f,0.6f,1.0f};
	GLfloat mat_speclular[] = {1.0,1.0,1.0,1.0};
	GLfloat mat_shininess[] = {50.0f};
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_speclular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);

	//set the light source properties
	GLfloat lightIntensity[] = {0.7f,0.7f,0.7f,1.0f};
	GLfloat light_position[] = {2.0f,6.0f,3.0f,0.0f};
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightIntensity);

	//set the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double winHt = 1.0;      //half -height of the window
	glOrtho(-winHt * 64/48.0,winHt * 64/48.0,-winHt,winHt,0.1,100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2.3,1.3,2,0,0.25,0,0.0,1.0,0.0);

	//start drawing
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//clear the screen
	glPushMatrix();
	glTranslated(0.4,0.4,0.6);
	glRotated(45,0,0,1);
	glScaled(0.08,0.08,0.08);
	jack();//draw the jack
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.6,0.38,0.5);
	glRotated(30,0,1,0);
	glutSolidTeapot(0.08);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.25,0.42,0.35);
	glutSolidSphere(0.1,15,15);//draw the sphere
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.4,0,0.4);
	table(0.6,0.02,0.02,0.3);  //draw the table
	glPopMatrix();

	wall(0.02);					//wall #1:in xz-plane
	glPushMatrix();
	glRotated(90,0.0,0.0,1.0);
	wall(0.02);
	glPopMatrix();

	glPushMatrix();
	glRotated(-90.0,1.0,0.0,0.0);
	 
	wall(0.02);
	glPopMatrix();
	glFlush();

}

void main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("Shaded example-3D scene");
	glutDisplayFunc(displaySolid);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor(0.1f,0.1f,0.1f,0.0f);
	glViewport(0,0,640,480);
	glutMainLoop();
}












