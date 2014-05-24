#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>		//Load the I/O head

const GLint screenWidth = 640,
			screenHeight = 480;
const GLint NUM_OF_POLY = 10;

typedef struct tagGLintPoint{
	int x;
	int y;
	float r,g,b;
}GLintPoint;

typedef struct tagPolyLine{
	int N;
	GLintPoint *points;
}PolyLine;

PolyLine pLines[NUM_OF_POLY];
int index=0;

void InitPolyLine(void)
{
	FILE	*fp;
	if((fp = fopen("dino.dat","r"))==NULL)
	{
		printf("cannot open file dino.dat");
		exit(0);
	}
	int N;
	while((fscanf(fp,"%d",&N)!=EOF))
	{
		pLines[index].N = N;
		pLines[index].points = new GLintPoint[10];
		for(int i = 0;i<abs(pLines[index].N);i++)
			fscanf(fp,"%d %d %f %f %f",&pLines[index].points[i].x,&pLines[index].points[i].y,\
			&pLines[index].points[i].r,&pLines[index].points[i].g,&pLines[index].points[i].b);
		index++;
	}

}
void Init(void)
{
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,screenWidth,0.0,screenHeight);

	InitPolyLine();
}

void RenderScene(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	for(int i = 0; i < index; i++)
	{
		int num;
		if(pLines[i].N>0)
		{
				glBegin(GL_LINE_STRIP);
				num = pLines[i].N;
		}else{
			glBegin(GL_LINE_LOOP);
			num = -pLines[i].N;
		}
		for(int j = 0; j < num; j+=2)
		{	
			glColor3f(pLines[i].points[j].r,pLines[i].points[j].g,pLines[i].points[j].b);
			glVertex2i(pLines[i].points[j].x,pLines[i].points[j].y);
			if(j+1<num)
			  glVertex2i(pLines[i].points[j+1].x,pLines[i].points[j+1].y);
			
		}
		glEnd();
	}
	
	glFlush();
}

void main(int argc, char ** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(screenWidth,screenHeight);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Polylines Files");
	glutDisplayFunc(RenderScene);
	Init();
	glutMainLoop();

}