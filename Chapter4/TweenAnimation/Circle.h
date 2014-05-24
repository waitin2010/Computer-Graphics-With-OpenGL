#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#define _USE_MATH_DEFINES // for C
#include <math.h>

#include "Point2.h"

void RenderCircle(Point2 center,float radius)
{
	glBegin(GL_LINE_LOOP);
	for(float t = 0.0; t<2* M_PI; t+= 0.1)
		glVertex2f(radius * cos(t)+center.x,radius*sin(t)+center.y);
	glEnd()	;
}
Point2 Triangle[3]={
	Point2(-1.0,-0.5),Point2(0.5,-0.5),Point2(0.0,0.5)
};
Point2 InnerTriangle[3];
Point2 NineTriangle[3];
Point2 center(0.0,0.0);
float radius=0.5;
void RenderPoints()
{
	glPointSize(4.0);
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		for(int i=0;i<3;i++)
			glVertex2f(InnerTriangle[i].x,InnerTriangle[i].y);
	glEnd();
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_POINTS);
	for(int i=0;i<3;i++)
		glVertex2f(NineTriangle[i].x,NineTriangle[i].y);
	glEnd();
	glColor3f(0.0,0.0,0.0);
}
void RenderTriangle(Point2 *triangle)
{
	glBegin(GL_LINE_LOOP);
		for(int i =0;i<3;i++)
			glVertex2f(triangle[i].x,triangle[i].y);
	glEnd();
}
void GetCenterAndRadius(Point2 *triangle, Point2&center, float& radius)
{
	float temp = (triangle[2].x-triangle[1].x)*(triangle[0].x-triangle[2].x)+(triangle[2].y-triangle[1].y)*(triangle[0].y-triangle[2].y);
	temp /= (triangle[0].y-triangle[1].y)*(triangle[0].x-triangle[2].x)+(triangle[1].x-triangle[0].x)*(triangle[0].y-triangle[2].y);
	center.x = triangle[0].x +triangle[1].x + temp * (triangle[0].y-triangle[1].y);
	center.x/=2;
	center.y = triangle[0].y +triangle[1].y + temp * (triangle[1].x-triangle[0].x);
	center.y/=2;
	radius = sqrt(temp*temp + 1);
	radius *= sqrt((triangle[1].x - triangle[0].x)*(triangle[1].x - triangle[0].x)+(triangle[1].y - triangle[0].y)*(triangle[1].y - triangle[0].y))/2;
}
void GetInnerCircle(Point2 *triangle,Point2 *InerTriangle)
{
	float lena = sqrt((triangle[1].x - triangle[0].x)*(triangle[1].x - triangle[0].x)+(triangle[1].y - triangle[0].y)*(triangle[1].y - triangle[0].y));
	float lenb = sqrt((triangle[2].x - triangle[1].x)*(triangle[2].x - triangle[1].x)+(triangle[2].y - triangle[1].y)*(triangle[2].y - triangle[1].y));
	float lenc = sqrt((triangle[0].x - triangle[2].x)*(triangle[0].x - triangle[2].x)+(triangle[0].y - triangle[2].y)*(triangle[0].y - triangle[2].y));
	float La = (lena - lenb + lenc)/2;
	float Lb = (lena + lenb - lenc)/2;
	InnerTriangle[0].x = triangle[0].x + La * (triangle[1].x - triangle[0].x)/lena;
	InnerTriangle[0].y = triangle[0].y + La * (triangle[1].y - triangle[0].y)/lena;

	InnerTriangle[1].x = triangle[1].x + Lb * (triangle[2].x - triangle[1].x)/lenb;
	InnerTriangle[1].y = triangle[1].y + Lb * (triangle[2].y - triangle[1].y)/lenb;

	InnerTriangle[2].x = triangle[0].x - La * (triangle[0].x - triangle[2].x)/lenc;
	InnerTriangle[2].y = triangle[0].y - La * (triangle[0].y - triangle[2].y)/lenc;

}

void GetNineCircle(Point2 *triangle,Point2 *NineTriangle)
{
	NineTriangle[0].x = triangle[0].x + (triangle[1].x - triangle[0].x)/2;
	NineTriangle[0].y = triangle[0].y + (triangle[1].y - triangle[0].y)/2;

	NineTriangle[1].x = triangle[1].x + (triangle[2].x - triangle[1].x)/2;
	NineTriangle[1].y = triangle[1].y + (triangle[2].y - triangle[1].y)/2;

	NineTriangle[2].x = triangle[2].x + (triangle[0].x - triangle[2].x)/2;
	NineTriangle[2].y = triangle[2].y + (triangle[0].y - triangle[2].y)/2;

}