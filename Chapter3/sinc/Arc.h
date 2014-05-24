#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>

#include <math.h>

void RenderArc(float start,float end,float radius,float cx = 0, float cy = 0)
{
	glBegin(GL_POINTS);
	for(float t = start; t < end; t += 0.001) 
		glVertex2f(radius*cos(t)+ cx,radius*sin(t)+ cy);
	glEnd();
}

void DrawBasic(float W, float H)
{
	glBegin(GL_LINE_STRIP);
		glVertex2f(-W,0);
		glVertex2f(-W,-H);
		glVertex2f(W,-H);
		glVertex2f(W,0);
	glEnd();
}
void renderArc1(float W, float H)
{
	DrawBasic(W,H);
	RenderArc(0,M_PI,W);
}

void renderArc2(float W, float H)
{
	DrawBasic(W,H);
	RenderArc(0,M_PI/3,2*W,-W,0);
	RenderArc(2*M_PI/3,M_PI,2*W,W,0);
}

void renderArc3(float W, float H, float f)
{
	float angle = asin((f*W)/((1 + f)*W));
	DrawBasic(W,H);
	RenderArc(0,M_PI/2 - angle,W);
	RenderArc(M_PI/2+angle,M_PI,W);
	RenderArc(M_PI,3 * M_PI/2 - angle,f*W,f*W,(1+f)*W*cos(angle));
	RenderArc(3*M_PI/2 + angle, 2 * M_PI, f*W, -f*W,(1+f)*W*cos(angle));
}

void ElectrostaticField(float n, float m, float a)
{
	for(float i = 1.0; abs(i-m)>0.001; i+=0.2)
	{
		RenderArc(0.0, 2*M_PI,a * i, 0, a*sqrt((double)i * i - 1));
		RenderArc(0.0, 2 * M_PI, a * i, 0, -a*sqrt((double)i * i - 1));
	}
	for(float j = 1.0; abs(j-n)>0.001; j+= 0.2)
	{
		RenderArc(0.0, 2*M_PI,a*sqrt((double)j * j - 1), a * j , 0);
		RenderArc(0.0, 2*M_PI,a*sqrt( (double)j * j - 1), -a * j , 0);
	}
}

void Smith()
{
	for(float m = 0.0; m < M_PI; m+= 0.5)
		RenderArc(0.0, 2 * M_PI, m, 1-m, 0);
	for(float n = 0.0; n < M_PI; n += 0.3)
	{
		RenderArc(0.0, 2 * M_PI, n, 1, n);
		RenderArc(0.0, 2 * M_PI, n, 1, -n);
	}
}

void Tiling() // Render the Tiling sequence
{
	RenderArc(M_PI, 3 * M_PI /2, 1.0,0.5,0.5);
	RenderArc(3 * M_PI/2, 2*M_PI, 1.0,-0.5,0.5);
	RenderArc(0,  M_PI /2, 1.0,-0.5,-0.5);
	RenderArc(M_PI/2,  M_PI, 1.0,0.5,-0.5);
}

void Truchlet()
{
	int num = rand();
	if(num%2){
		RenderArc(0, M_PI /2, 0.5,-0.5,-0.5);
		RenderArc(M_PI, 3*M_PI /2, 0.5,0.5,0.5);
	}else{
		RenderArc(3 * M_PI/2, 2*M_PI, 0.5,-0.5,0.5);
		RenderArc(M_PI/2,  M_PI, 0.5,0.5,-0.5);

	}
}