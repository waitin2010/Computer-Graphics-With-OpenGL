#define _USE_MATH_DEFINES
#include "WindowAndViewport.h"
#include <math.h>


void renderRegularPolygon(float radius,int N = 100,float theta = 0.0)  //Draw the N Regular Polygan
{
	float a = 2 * M_PI / N;
	glBegin(GL_LINE_LOOP);
		for(float t = 0.0; t < 2 * M_PI; t += a  )
		{
			glVertex2f(radius * cos(t + theta), radius * sin (t + theta));
		}
	glEnd();
}

void hexSwirl()
{
	float aspect = 1.05;
	float step = 2 * M_PI /36;
	float radius = 1.0;
	/*setWindow(-radius,radius,-radius,radius);
	setViewport(100,100,640,480);*/
	for(float t = 0.0; t <= 2 * M_PI; t+= step)
	{
		renderRegularPolygon(radius,6,t);
		radius /= aspect;
	}
	
}
void hexSwirlLayout()
{
	setWindow(-0.5,0.5,-0.5,0.5);
	for(int i = 0; i< 5; i++)
		for(int j = 0; j<4; j++)
		{
			int L = 120;
			setViewport(i*L,L+i * L ,j*L,L + j*L);
			hexSwirl();
		}
}

