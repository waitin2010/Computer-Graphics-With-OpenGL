#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "camera.h"
#include "mesh.h"
#include "kdtree.h"
#include "ray.h"

Camera cam; // make a global camera object
Scene scene;
KDTree tree;
Ray ray;
Triangle tri;
void Init(void)
{
	glClearColor(1.0,1.0,1.0,0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
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

	
	cam.set(40, 40, 40, 0, 0, 0, 0, 1, 0); // make the initial camera
	cam.setShape(30.0f, 64.0f/48.0f, 0.5f, 1000.0f);

	tree.Build(&scene);
	//test the intersection between the ray and triangle
	/*ray.position = Point3(0,0,0);
	ray.setDirection(vector3(1,0,0));
	Point3 hitPoint;
	tri.setTriangle(Point3(2,1,0),Point3(2,0,1),Point3(2,0,0),0);
	tri.Intersect(ray,hitPoint);
	printf("%f %f %f\n",hitPoint.x,hitPoint.y,hitPoint.z);*/
	
	glViewport(0,0,480,480);
}

void meshFloor()
{
	static float xmax =5.0, zmax = 5.0;
	glLineWidth(2.0);
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINES); 
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,1.0,0.0);
	glEnd();
	glLineWidth(1.0);
	glColor3f(0.0,0.0,0.0);
	for(float x = -5.0; x < 5.0; x += 0.5)
	{
		glBegin(GL_LINES);
		glVertex3f(x,0,-zmax);
		glVertex3f(x,0,zmax);
		glEnd();
	}
	for(float z = -5.0; z < 5.0; z += 0.5)
	{
		{
			glBegin(GL_LINES);
			glVertex3f(-xmax,0,z);
			glVertex3f(xmax,0,z);
			glEnd();
		}
	}
	
}
void render()
{
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0,0.0,0.0);
	//scene.Draw();
	KDTreeNode* templist = tree.GetRoot();
	tree.Draw(templist);
	//ray.draw(14);
	//meshFloor();
	glutSwapBuffers();	
};


//<<<<<<<<<<<<<<<<<<<<<<<< myKeyboard >>>>>>>>>>>>>>>>>>>>>>
void myKeyboard(unsigned char key, int x, int y)
{
	switch(key)
	{	

		// controls for camera
	case 'L':      cam.slide(.2, 0, 0); break;// slide camera right
	case 'l':	   cam.slide(-0.2, 0, 0); break; // slide camera left

	case 'U':      cam.slide(0, .2, 0); break;// slide camera up
	case 'u': cam.slide(0, -0.2, 0); break; // slide camera down

	case 'F':    cam.slide(0,0, 0.2); break; // slide camera forward
	case 'f': cam.slide(0,0,-0.2); break; //slide camera back	
		// add up/down and left/right controls	
	case 'P':      cam.pitch(-1.0); break;
	case 'p':		cam.pitch( 1.0); break;
		// add yaw controls
	case 'Y':      cam.yaw(-1.0); break;
	case 'y':    cam.yaw( 1.0); break;
		// add roll controls
	case 'R':      cam.roll(1.0); break;
	case 'r': cam.roll(-1.0); break;

	case 'C':		
		ray.randRay();
		scene.intersect(ray);
		Point3 hitPoint;
		printf("the hit objects using the rope kdtree:\n");
		tree.TraversalRope(tree.GetRoot(),ray,ray.position);
		printf("\n\n");
		//cam.set(ray.position,Point3(0,0,0),vector3(0,1,0));
		break;
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
	//glutIdleFunc(render);
	glutKeyboardFunc(myKeyboard);
	Init();
	glutMainLoop();
}












