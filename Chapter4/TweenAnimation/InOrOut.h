#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

#include "Point2.h"

Point2 Perpendicular(Point2 &p)
{
	return Point2(p.y,-p.x);
}

float Dot(Point2 &a, Point2 &b)
{
	return a.x*b.x + a.y * b.y;
}

int num = 0; //store the number of the points of the polygon
Point2 *pointlist; //store the points of the polygon
Point2 Q;   //the test point which is in or out of the polygon
void GetPointList()
{
	num = 6;  // the max num is setted to 8
	pointlist = new Point2[num];
	for(int i = 0; i< num;i++)
	{
		
		pointlist[i].set(cos(M_PI/3*i),sin(M_PI/3*i));

	}
	Q.set((float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
}

void DrawPolygon()
{
	glBegin(GL_LINE_LOOP);
		for(int i =0;i<num;i++)
			glVertex2f(pointlist[i].x,pointlist[i].y);
	glEnd();
	glPointSize(4.0);
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(Q.x,Q.y);
	glEnd();
	glColor3f(0.0,0.0,0.0);
}

void testQ()
{
	Point2 a;
	Point2 N;
	Point2 s;
	int i;
	for( i = 0; i< num; i++)
	{
		
		a.set(Q.x - pointlist[i].x,Q.y - pointlist[i].y);
		s.set(pointlist[(i+1)%num].x - pointlist[i].x,pointlist[(i+1)%num].y - pointlist[i].y);
		N.set(Perpendicular(s));
		if(Dot(a,N)>0)
		{
			printf("Outer1\n");
			break;
		}
	}
	if(i>=num)
		printf("Inner!\n");
}

