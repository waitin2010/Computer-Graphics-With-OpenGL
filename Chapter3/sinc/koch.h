#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "Garland.h"

typedef struct LNode
{
	Point2 point;
	struct LNode * next;
}LNode,*PointList;

PointList pointlist;
void insertList(PointList p,PointList q);
void InitList()
{
	float angle = 2 * M_PI /3;
	Point2 points[3];
	int i=0;
	for(float t = 0.0; t < 2 * M_PI; t += angle)
		points[i++].set(1.0 * sin(t),1.0 * cos(t));
	pointlist = new LNode;
	pointlist->point.set(points[0]);
	pointlist->next = NULL;

	PointList p = new LNode;
	p->point.set(points[1]);
	insertList(pointlist,p);
    p = new LNode;
	p->point.set(points[2]);
	insertList(pointlist->next,p);
	p = new LNode;
	p->point.set(points[0]);
	insertList(pointlist->next->next,p);

}
void insertList(PointList p,PointList q)
{
	q->next = p->next;
	p->next = q;
}

void GetDividedPoint(Point2 &p1,Point2 &p2,Point2 &ans,int n)//Get the n divided points
{
	
	 ans.set(p2.x * (n-1)/n + p1.x * 1/n,p1.y * 1/n  + p2.y *  (n-1)/n );
	
}

void GetTriPoint(Point2 &p1,Point2 &p2,Point2 &ans)
{
	Point2 Temp;
	GetDividedPoint(p1,p2,Temp,3);
	float angle = 2*M_PI /3;
	Point2 center(p2.x - Temp.x,p2.y - Temp.y);
	Point2 Answer(center.x * cos(angle) - center.y * sin(angle),center.x * sin(angle)+center.y*cos(angle));
	ans.set(Answer.x + Temp.x,Answer.y+Temp.y);
}
void koch(int n)
{
	for(int i =0;i<n;i++)
	{
		PointList p = pointlist;
		while(p->next)
		{
			PointList q;
			Point2 temp,p1,p2;
			p1.set(p->point);
			p2.set(p->next->point);
			GetDividedPoint(p2,p1,temp,3);
			q = new LNode;
			q->point.set(temp);
			insertList(p,q);
			p = p->next;
			GetTriPoint(p1,p2,temp);
			q = new LNode;
			q->point.set(temp);
			insertList(p,q);
			p= p->next;
			GetDividedPoint(p1,p2,temp,3);
			q = new LNode;
			q->point.set(temp);
			insertList(p,q);
			p= q->next;
		}
	}

}
void renderPointList()
{
	InitList();
	PointList p = pointlist;
	
	koch(5);
	while(p->next)
	{
		glBegin(GL_LINES);
			moveTo(p->point);
			lineTo(p->next->point);
		glEnd();
		p = p->next;
	}
}