#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Point2.h"
#pragma once

Point2 Tree[11] = {
	Point2(0.2,0.0),Point2(0.2,0.2),Point2(1.0,0.2),Point2(0.2,0.7),
	Point2(0.7,0.7),Point2(0.0,1.0),Point2(-0.7,0.7),Point2(-0.2,0.7),
	Point2(-1.0,0.2),Point2(-0.2,0.2),Point2(-0.2,0.0)
};
Point2 F[11] = {
	Point2(0.1,0.0),Point2(0.1,0.5),Point2(0.5,0.5),Point2(0.5,0.6),
	Point2(0.1,0.6),Point2(0.1,0.7),Point2(1.0,0.7),Point2(1.0,0.9),
	Point2(-0.3,0.9),Point2(-0.3,0.5),Point2(-0.3,0.0)
};

void moveTo(Point2 p)
{
	CP.set(p);
}

void lineTo(Point2 p)
{
	glBegin(GL_LINES);
	glVertex2f(CP.x,CP.y);
	glVertex2f(p.x,p.y);
	glEnd();
	glFlush();
	CP.set(p);
}

void RenderPoly(Point2 array[],int n)
{
	moveTo(array[0]);
	for(int i = 1; i < n; i++)
		lineTo(array[i]);
}

float lerp(float a, float b, float t)
{
	return a + ( b - a ) * t;
}

Point2 Tween(Point2 &p1, Point2 &p2, float t)
{
	Point2 temp;
	temp.x = lerp(p1.x,p2.x,t);
	temp.y = lerp(p1.y,p2.y,t);
	return temp;
}

void RenderTween(Point2 A[], Point2 B[], int n, float t)
{
	for(int i = 0; i<n;i++)
	{
		Point2 p;
		p.set(Tween(A[i],B[i],t));
		if(i==0)
			moveTo(p);
		else
			lineTo(p);
	}
}