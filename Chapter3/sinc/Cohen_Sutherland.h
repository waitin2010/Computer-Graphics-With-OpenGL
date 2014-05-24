#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Garland.h"   //introduce the point2 structure member

typedef struct tagRealRect
{
	float left;
    float right;
	float buttom;
	float top;
}RealRect;

void DrawWindow(RealRect window)
{
	glBegin(GL_LINE_LOOP);
		glVertex2f(window.left,window.buttom);
		glVertex2f(window.right,window.buttom);
		glVertex2f(window.right,window.top);
		glVertex2f(window.left,window.top);
	glEnd();
}
unsigned char CodeGet(Point2 &p,RealRect window)
{
	unsigned char code = 0;
	if( p.x < window.left)		code |= 8;  //Set the 3th bit
	if( p.y > window.top )		code |= 4;  //Set the 2th bit
	if( p.x > window.right)		code |= 2;	//set the 1th bit
	if(	p.y < window.buttom)	code |= 1;
	return code;
}
void ChopLine(Point2 &p,unsigned char code,float dely, float delx,	RealRect &window)
{
	if(code & 8)//Left
	{
		p.y += ( window.left - p.x ) * dely / delx;
		p.x = window.left;
	}else if(code & 2)//Right
	{
		p.y += (window.right - p.x ) * dely / delx;
		p.x = window.right;
	}else if(code & 1)//Buttom
	{
		p.x += (window.buttom - p.y) * delx /dely;
		p.y = window.buttom;
	}else if(code & 4)//Top
	{
		p.x += (window.top - p.y ) * delx /dely;
		p.y = window.top;
	}
}
void DrawLine(Point2 &p1,Point2	&p2,int In)
{
	if(In == 1)//the segment is in the window
		glColor3f(1.0,0.0,0.0);
	else
		glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex2f(p1.x,p1.y);
		glVertex2f(p2.x,p2.y);
	glEnd();
}
int clipSegment(Point2 &p1,Point2 &p2,RealRect &window)
{
	
	unsigned char code_p1,code_p2;
	float dely,delx;
	Point2 temp;
	
	delx = p2.x - p1.x;
	dely = p2.y - p1.y;
	//clip the line segment
	do
	{
		//Init the codes for the points p1 and p2
		code_p1 = CodeGet(p1,window);
		code_p2 = CodeGet(p2,window);

		if(!(code_p1|code_p2))//Inner 
			return 1;
		if(code_p1&code_p2)//Outer
			return 0;
		if(code_p1)//P1 is out of window
		{
			temp.set(p1);
			ChopLine(p1,code_p1,dely,delx,window);
			DrawLine(temp,p1,0);
		}else
		{
			temp.set(p2);
			ChopLine(p2,code_p2,dely,delx,window);
			DrawLine(temp,p2,0);
		}
	}while(1);
}
void randPoint(Point2 &p)
{
	float x = 4.0 * rand()/RAND_MAX;
	float y = (float)rand()/RAND_MAX * 4.0;
	p.set(x,y);
}
void renderClip()
{
	RealRect window = {1.0,3.0,1.0,3.0};
	DrawWindow(window);
	int count = 0;
	Point2 p1;
	Point2 p2;
	
	while(count++<=10)
	{
		randPoint(p1);
		randPoint(p2);
		if(clipSegment(p1,p2,window))
			DrawLine(p1,p2,1);
		else 
			DrawLine(p1,p2,0);
	}

}