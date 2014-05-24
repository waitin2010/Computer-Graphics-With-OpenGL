#include "Mesh.h"


Mesh::Mesh(void)
{
	numVerts = 0;
	pt = NULL;
	numNormals = 0;
	norm = NULL;
	numFaces = 0;
	face = NULL;
}


Mesh::~Mesh(void)
{
	numVerts = 0;
	delete[] pt;
	numNormals = 0;
	delete[] norm;
	numFaces = 0;
	delete[] face;
}

int Mesh::readFile(char *fileName)
{
	fstream infile;
	infile.open(fileName,ios::in);
	if(infile.fail()) return -1;
	if(infile.eof()) return -1;
	infile>>numVerts>>numNormals>>numFaces;
	pt = new Point3[numVerts];
	norm = new vector3[numVerts];
	face = new Face[numFaces];

	if(!pt || !norm || ! face) return -1;
	for(int p =0; p < numVerts; p++)
		infile>>pt[p].x>>pt[p].y>>pt[p].z;
	for(int n = 0; n < numNormals; n++)
		infile>>norm[n].x>>norm[n].y>>norm[n].z;
	for(int f = 0; f < numFaces; f++)
	{
		infile>>face[f].nVerts;
		face[f].vert = new VertexID[face[f].nVerts];
		for(int i = 0; i < face[f].nVerts; i++)
			infile>>face[f].vert[i].vertIndex;
		for(int i = 0; i < face[f].nVerts; i++)
			infile>>face[f].vert[i].normIndex;
	}
	return 1;
}

void Mesh::draw()
{
	for(int f = 0; f < numFaces; f++)
	{
		glBegin(GL_LINE_LOOP);
		for(int v = 0; v < face[f].nVerts; v++)
		{
			int in = face[f].vert[v].normIndex;
			int iv = face[f].vert[v].vertIndex;
			glNormal3f(norm[in].x,norm[in].y,norm[in].z);
			glVertex3f(pt[iv].x,pt[iv].y,pt[iv].z);
		}
		glEnd();
	}
}

//void Mesh::intersect(Ray &ray,Point3 &hitPoint)
//{
//
//
//	Point3 v1,v2,v3;
//	int id=0;
//	TriangleNode *triangle = new TriangleNode();
//	for(int i =0 ; i< numFaces; i++)
//	{
//		int nVerts =face[i].nVerts;
//		v1 = pt[face[i].vert[0].vertIndex];
//		for(int j=1; j<nVerts-1;j++)
//		{
//			v2 = pt[face[i].vert[j].vertIndex];
//			v3 = pt[face[i].vert[j+1].vertIndex];
//			triangle->SetNodeValue(v1,v2,v3,id);
//			triangle->Tri->Intersect(ray,hitPoint);
//
//		}
//
//	}
//}