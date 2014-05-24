#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <gl/glu.h>
#include "common.h"
#include "ray.h"
#include "Mesh.h"

//define vertex
#define vertex Point3

#define maxTrianglesPerleafNode 20
#define minTrianglesPerleafNode 5
#define Dimension 3

#define kdBefore -1
#define kdAfter 1
#define kdIntersection 0

#define kdPlaneYZ   0
#define kdPlaneXZ   1
#define kdPlaneXY   2

#define cTraversal  0.3
#define numofObject 120
#define LEFT 0
#define PATITION 1
#define MODEL GL_LINE_LOOP

//define Triangle class
class Triangle
{
public:
	Triangle(){}
	~Triangle(){}
	Triangle(Point3 &v1, Point3 &v2,Point3 &v3,int id);
	void setTriangle(Point3 &v1, Point3 &v2,Point3 &v3,int id);
	vertex GetVertex(int i);
	void Draw();
	bool Intersect(Ray &ray,Point3 &hitPoint);
	int getObjectID();
private:
	vertex m_Vertex[3];
	vertex normal;
	int m_ObjectID;
};

class TriangleNode
{
public:
	void SetNextNode(TriangleNode* nextNode);
	TriangleNode* GetNext();
	vertex GetVertex(int i);
	void SetNodeValue(Point3 &v1, Point3 &v2,Point3 &v3,int id);
public:
	Triangle* Tri;
	TriangleNode* next;
};

class TriangleList
{
public:
	TriangleList();
	int GetCount();
	TriangleNode* GetHead();
	void DeleteHead();
	void append(TriangleNode* triangle);
	void Draw();	
private:
	TriangleNode *m_List;//point to the triangle list
	int m_Count;    //the number of the triangle in the list
};

class KDTreeNode
{
public:
	KDTreeNode(){};
	void SetAxis( int Axis);
	int GetAxis();
	void SetLeft(KDTreeNode* left);
	KDTreeNode* GetLeft();
	void SetRight(KDTreeNode* right);
	KDTreeNode* GetRight();
	void SetTriangleList(TriangleList *List);
	TriangleList* GetTriangleList();
	void SetSplitPosition(real split);
	real GetSplitPosition();	
public:
	aabb box;
	int Type;
	union
	{
		//partionNode
		struct 
		{
			KDTreeNode* m_lchild;
			KDTreeNode* m_rchild;
			int m_Axis;
			real m_Split;
		};
		//leafNode
		struct
		{
			TriangleList* m_List;
			
			KDTreeNode *ropes[6];
		};
	};
	
};
struct SplitNode
{
	real splitpos;
	int nlcount,nrcount;
	SplitNode* next;
};
class SplitList
{
public:
	SplitList();
	void InsertSplitPos( real splitPos );
	SplitNode *GetHead();
public:
	SplitNode *m_SList;
};
class Scene
{
	
public:
	Scene();
	void append();
	aabb GetBoundingBox();
	TriangleList* GetTriangles();
	void Draw();
	void intersect(Ray &ray);
private:
	void LoadScene();
	void SetTriangle();
	bool intersectMesh(Ray &ray, Mesh &object);
private:
	aabb m_box;
	TriangleList *m_triangles;
	Mesh m_objects[numofObject];
};

enum Side{Left=0,Right,Top,Bottom,Front,Back };

class KDTree
{
public:
	KDTree();
	~KDTree(){};
	void Build(Scene* scence);
	KDTreeNode* GetRoot();
	float caculateSingleVoxelCost(int numofTriangles,aabb &box);
	float caculateCost(SplitNode *split,aabb &frontBox, aabb &backBox);
	bool findOptimalSplitPositon(TriangleList *list,aabb &box,int depth,real &splitPosition,aabb &frontBox, aabb &backBox);
	int partitionTriangle(TriangleNode *triangle,int depth,int splitPosition);
	void Subdivide(KDTreeNode& node,TriangleList *list,aabb& box,int depth);
	void Draw(KDTreeNode* templist);
	void DrawSplitPlane(KDTreeNode *templist);
	//Rope construction
	void Optimize(KDTreeNode* R,Side s,aabb AABB);
	void ProcessNode(KDTreeNode* N,KDTreeNode *RS[]);
	KDTreeNode* findLeaf(KDTreeNode* N,Point3 pos);
	int Intersect(TriangleNode* head,Ray &ray);
	void GetBoxVertex(aabb box,Point3* Pos);
	bool IntersectPlane(Ray &ray,Point3 ray_Pos,Point3 p0,Point3 p1,Point3 p2,Point3 p3,Point3 &hitPoint);
	void TraversalRope(KDTreeNode *N, Ray &ray,Point3 hitPoint);

private:
	KDTreeNode * m_Root;
	
};