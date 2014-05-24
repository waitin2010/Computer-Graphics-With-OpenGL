#define _USE_MATH_DEFINES
#include <math.h>
#include "WindowAndViewport.h"
#pragma once
class GLintPoint
{
public:
	GLint x,y;
};

class Point2
{
public:
	float x,y;
	void set(float dx, float dy)
	{
		x = dx;
		y = dy;
	}
	void set(Point2 &p)
	{
		x = p.x;
		y = p.y;
	}
	Point2(float xx, float yy)
	{
		x = xx;
		y = yy;
	}
	Point2()
	{
		x = y = 0;
	}
};

Point2 currPos;
Point2 CP;

void renderStar(int N, float radius);
void moveTo(Point2 p)
{
	CP.set(p);

}
void moveTo(float x, float y)
{
	CP.set(x,y);
}
void  lineTo(Point2 p)
{
	glBegin(GL_LINES);
		glVertex2f(CP.x,CP.y);
		glVertex2f(p.x,p.y);
	glEnd();
	CP.set(p);
}

void lineTo(float x, float y)
{
	glBegin(GL_LINES);
		glVertex2f(CP.x,CP.y);
		glVertex2f(x,y);
	glEnd();
	CP.set(x,y);
}
void rosette(int N, float radius)
{
	Point2 *pointList = new Point2[N];
	GLfloat	theta = (2.0f * M_PI)/N;
	for(int c = 0; c < N; c++)
	{
		pointList[c].set(radius * sin(theta *c), radius * cos(theta * c));
	}

	for(int i = 0; i< N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			moveTo(pointList[i]);
			lineTo(pointList[j]);
		}
	}


}


void renderStar(int N , float radius)
{
	Point2 *pointList = new Point2[N];
	GLfloat	theta = (2.0f * M_PI)/N;
	for(int c = 0; c < N; c++)
	{
		pointList[c].set(radius * sin(theta *c), radius * cos(theta * c));
	}
	for(int i = 0; i< N; i++)
	{
		moveTo(pointList[i]);
		lineTo(pointList[(i+2)%N]);
	}
}

void renderTag()
{
	Point2 pointlistSmall[3];
	Point2 pointlistBig[3];
	GLfloat	theta = (2.0f * M_PI)/3;
	for(int c = 0; c< 3;c++)
	{
		pointlistBig[c].set(1.0 * sin(theta*c),1.0 * cos(theta * c));
		pointlistSmall[c].set(0.15 * sin(theta * c),0.15 * cos(theta * c));
	}
	for(int i = 0;i<3;i++)
	{
		for(int j = 0;j<3;j++)
		{
			if(i != j)
			{
				glBegin(GL_LINES);
				glVertex2f(pointlistSmall[i].x * cos(M_PI) - pointlistSmall[i].y * sin(M_PI),pointlistSmall[i].x * sin(M_PI) + pointlistSmall[i].y * cos(M_PI));
				glVertex2f(pointlistBig[j].x,pointlistBig[j].y);
				glEnd();
			}
		}
	}
}

void renderMC(Point2 p,float length, float angle)
{
	GLfloat  theta = (2.0f * M_PI)/3;
	Point2 pointlist[6];
	pointlist[0].set(p);
	pointlist[1].set( length /tan(angle) ,pointlist[0].y+length);
	pointlist[5].set(-pointlist[1].x,pointlist[1].y);
	pointlist[2].set(pointlist[1].x,pointlist[1].y+ length);
	pointlist[4].set(-pointlist[2].x,pointlist[2].y);
	pointlist[3].set(pointlist[0].x,pointlist[0].y+length);
	/*for(int i =0;i< 6; i++)
	{
		moveTo(pointlist[i]);
		lineTo(pointlist[(i+1)%6]);
	}*/
	for(float t = 0.0;t< 2 * M_PI; t +=theta)
	{
		
			for(int i =0; i < 6;i++)
			{
					moveTo(pointlist[i].x * cos(t) - pointlist[i].y * sin(t),pointlist[i].x * sin(t) + pointlist[i].y * cos(t));
					lineTo(pointlist[(i+1)%6].x * cos(t)-pointlist[(i+1)%6].y * sin(t),pointlist[(i+1)%6].x * sin(t)+pointlist[(i+1)%6].y * cos(t));
			}
		


	}

}
void rendGarland()
{
	setWindow(-1.0,1.0,-1.0,1.0);
	setViewport(0.0,250,100,350);
	rosette(5,0.66f);

	setWindow(-3.0,3.0,-3.0,3.0);
	setViewport(200,500,100,350);
	renderMC(Point2(0,0.2),0.8,M_PI/6);

	setWindow(-2.0,2.0,-2.0,2.0);
	setViewport(450,700,100,350);
	renderTag();

}
