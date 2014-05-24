#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Garland.h"

void renderLine()   //render one triangle line
{
	moveTo(0.0,0.0);
	lineTo(1.0,1.0);
}

void renderParabola(float a)
{
	float step = 0.001;
    moveTo(0.0,0.0);
	for(float x = 0.0; x <= 1.0; x+=step)
	{
		float y = 4 * a * x * ( 1 - x);
		lineTo(x,y);
	}
}
void renderLogicDiagram(float x, float a)
{
	//x0:the initial number for the Logic Diagram
	//y: the propotioanal number of the function
	float y;//the variant stands for the function output
	int count =0;
	renderLine();
	renderParabola(a);
	moveTo(x,0);
	y = 4 * a * x * ( 1 - x);
	while( abs(y-x)>0.01&&count++<=100)
	{
		lineTo(x,y);
		lineTo(y,y);
		x = y;
		y = 4 * a * x * ( 1 - x); 
	}
}
