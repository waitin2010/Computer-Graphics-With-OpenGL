#pragma  once

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <stdlib.h>
#include "kdtree.h"
#include "common.h"

class Ray
{
public:
	Point3 position;
	vector3 direction;
	Ray()
	{
		randRay();
	}
	void setPosition(Point3 pos)
	{
		position = pos;
	}
	void setDirection(vector3 Dir)
	{
		direction = Dir;
	}
	void draw(int length)
	{
		Point3 end;
	    end.x = position.x + direction.x * length;
		end.y = position.y + direction.y * length;
		end.z = position.z + direction.z * length;

		glLineWidth(2.0);
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_LINES);
			glVertex3f(position.x,position.y,position.z);
			glVertex3f(end.x,end.y,end.z);
		glEnd();
	}
	void randRay()
	{
		/*position.x =rand()%10;
		position.y = rand()%10;
		position.y = rand()%10;*/

		position.x =0;
		position.y =1;
		position.z =0;
		direction.x = rand()%5;
		direction.y = rand()%5;
		direction.z =rand()%5;

		direction.normalize();
	}
};
