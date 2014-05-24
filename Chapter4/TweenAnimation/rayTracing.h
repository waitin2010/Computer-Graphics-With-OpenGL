#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include "Point2.h"

typedef struct tagLine
{
	Point2 S;
	Point2 c;
}Line;
Line line; //the ray 

Point2 seg[2];//store the two end of the cross line
Point2 tempOfSeg[2];//store the min hit time of the polygon
Point2 tempOfSegOut[2];
float t;//store the hit time

typedef struct tagPolyGon
{
	int num;		//store the number of the points in the polygon
	Point2 *pointlist;
}PolyGon;


typedef struct tagScene
{
	PolyGon secretHouse;
	PolyGon *pilar;
	int num;
}Scene;
Scene scene;

void SceneInit()
{
	FILE *fp;
	float a,b;
	if((fp = fopen("scene.dat","r"))==NULL)
		exit(0);
	fscanf(fp,"%d",&scene.secretHouse.num);
	scene.secretHouse.pointlist = new Point2[4];
	for(int i = 0;i<scene.secretHouse.num;i++)
	{
		fscanf(fp,"%f %f",&a,&b);
		scene.secretHouse.pointlist[i].set(a,b);
	}
	fscanf(fp,"%d",&scene.num);
	scene.pilar = new PolyGon[scene.num];
	for(int i=0;i<scene.num;i++)
	{
		fscanf(fp,"%d",&scene.pilar[i].num);
		scene.pilar[i].pointlist = new Point2[scene.pilar[i].num];
		for(int j=0; j< scene.pilar[i].num; j++)
		{
			fscanf(fp,"%f %f",&a,&b);
			scene.pilar[i].pointlist[j].set(a,b);
		}
	}

	/*printf("Input the S of the line:\n");
	scanf("%f %f",&line.S.x,&line.S.y);
	printf("Input the C of the line:\n");
	scanf("%f %f",&line.c.x,&line.c.x);*/
	line.S.set(-0.15,0.0);
	line.c.set(0.2,-0.1);
	normalize(line.c);
}
void DrawPolygon(PolyGon &p)
{
	glBegin(GL_LINE_LOOP);
		for(int i = 0; i<p.num;i++)
			glVertex2f(p.pointlist[i].x,p.pointlist[i].y);
	glEnd();
}
void DrawScene()
{
	//Render the scene
	DrawPolygon(scene.secretHouse);
	for(int i = 0; i< scene.num;i++)
		DrawPolygon(scene.pilar[i]);

	//Render the ray with the red point at the start
	glColor3f(1.0,0.0,0.0);
	glPointSize(4.0);
	glBegin(GL_POINTS);
		glVertex2f(line.S.x,line.S.y);
	glEnd();
	glColor3f(0.0,0.0,0);
	glBegin(GL_LINES);
		glVertex2f(line.S.x,line.S.y);
		glVertex2f(line.S.x+2*line.c.x,line.S.y+2*line.c.y);
	glEnd();
}
int chopCI(double numer,double denom, double &tIn,double &tOut,PolyGon &p,int i)
{
	double tHit;
	if(denom <0)
	{
		tHit = numer /denom;
		if(tHit > tOut) return 0;
		else if (tHit >tIn)
		{
				tIn = tHit;
				tempOfSeg[0].set(p.pointlist[i]);
				tempOfSeg[1].set(p.pointlist[(i+1)%p.num]);
		}
	}else if(denom >0)
	{
		tHit = numer /denom;
		if(tHit <tIn) return 0;
		else if (tHit < tOut)
			{
				tOut = tHit;
				tempOfSegOut[0].set(p.pointlist[i]);
				tempOfSegOut[1].set(p.pointlist[(i+1)%p.num]);
		}
	}
	return 1;
}
int CyrusBeckclip(Line &seg,PolyGon &p,double &tin,double &tout)
{
	double numer ,denom;  //store the hit time
	double tIn = 0.0,tOut = 10000;
	Point2 c,temp,norm;
	c.set(seg.c);
	for(int i = 0;i< p.num; i++)
	{
		temp.set(p.pointlist[(i+1)%p.num].x - p.pointlist[i].x,p.pointlist[(i+1)%p.num].y - p.pointlist[i].y);
		norm.set(Perpendicular(temp));//get the vector n
		temp.set( p.pointlist[i].x - line.S.x,p.pointlist[i].y - line.S.y);
		numer = Dot(norm,temp);
		denom = Dot(norm,c);
		if(!chopCI(numer,denom,tIn,tOut,p,i))
			return 0;
	}
	tin = tIn;
	tout = tOut;
	return 1;
}

double FindFirstWall()
{
	double tin,tout,tHit = 10000.0;
	for(int i = 0; i<scene.num;i++)
	{
		CyrusBeckclip(line,scene.pilar[i],tin,tout);
		if(tin < tHit)
		{
			tHit = tin;
			seg[0].set(tempOfSeg[0]);
			seg[1].set(tempOfSeg[1]);
		}
	}
	if(tHit<=0.0)
	{
		CyrusBeckclip(line,scene.secretHouse,tin,tout);
		seg[0].set(tempOfSegOut[0]);
		seg[1].set(tempOfSegOut[1]);
		tHit = tout;
	}
	return tHit;
}

void updateRay()
{
	double tHit;
	Point2 norm,tempOfS,tempOfC;
	
	tHit = FindFirstWall();
	norm.set(seg[1].x - seg[0].x,seg[1].y - seg[0].y);
	norm.set(Perpendicular(norm));
	normalize(norm);
	tempOfS.set(line.S.x + line.c.x * tHit,line.S.y + line.c.y * tHit);
	double temp = Dot(line.c,norm);
	tempOfC.set(line.c.x - 2 * temp * norm.x,line.c.y - 2 * temp * norm.y);
	line.S.set(tempOfS);
	line.c.set(tempOfC);
	normalize(line.c);
}