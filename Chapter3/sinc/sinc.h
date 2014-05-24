#include "WindowAndViewport.h"


void renderSinc()
{
	glBegin(GL_LINE_STRIP);
	for(float x = - 4.0f; x < 4.0f; x += 0.1)
	{
		if( x == 0.0)
			glVertex2f(0.0,1.0);
		else
			glVertex2f(x,sin(pi * x)/(pi * x));

	}
	glEnd();
}
void renderLayout()
{
	for(int i = 0; i< 10; i++)
		for(int j = 0; j<10; j++)
		{
			if((i+j)%2==0)
				setWindow(-5.0,5.0,1.0,-0.3);
			else
				setWindow(-5.0,5.0,-0.3,1.0);
			glViewport(i*64,j*48,64,48);
			renderSinc();
		}
}
void renderAnimation()
{
	float cx = 0.0,cy = 0.0;
	float H = 1.0 ,W = 0.0,aspect = 0.7;
	glViewport(0,0,640,480);
	for(int frame = 0; frame <10; frame++)
	{
		setWindow(cx - W,cx + W,cy - H + 0.7, cy+ H);
		renderSinc();
		W += 0.2;
	}
}