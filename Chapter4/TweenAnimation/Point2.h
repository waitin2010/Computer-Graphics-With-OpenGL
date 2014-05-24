#pragma once 

class Point2
{
public:
	Point2(){ x = 0.0; y = 0.0;}
	Point2(float xx, float yy){ x = xx; y = yy;}
	Point2(Point2 &p){ x = p.x; y = p.y;}
	~Point2(){}

	void set(float xx, float yy)
	{
		x = xx;
		y = yy;
	}
	void set(Point2 &p)
	{
		x = p.x;
		y = p.y;
	}
public:
	float x,y;
};

Point2 CP;

void normalize(Point2 & a)
{
	double len = sqrt(a.x * a.x + a.y * a.y);
	a.set(a.x/len,a.y/len);
}