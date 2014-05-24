#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <fstream>
using namespace std;
//Point3
class Point3
{
public:
	float x,y,z;

};
//Vector3
class Vector3
{
public:
	float x,y,z;
};
//VertexID
class VertexID
{
public:
	int vertIndex;			//index in the verctor list
	int normIndex;			//norm index of the point
};

//Face
class Face
{
public:
	int nVerts;			  //num of verctors in the Face
	VertexID *vert;		 //list of vector and norm
	Face(){nVerts=0;vert = NULL;}
	~Face(){delete[] vert;nVerts = 0;}
};
class Mesh
{
private:
	int numVerts;	//the number of vectors in the grid
	Point3* pt;		//3d points array
	int numNormals; //the number of norm in the grid
	Vector3 *norm;  //norm array
	int numFaces;		//the number of Face in the grid
	Face *face;
public:
	Mesh(void);
	~Mesh(void);
	int readFile(char * fileName);   //Read the grid's datas from the file
	void draw();					//render the mesh object
};

