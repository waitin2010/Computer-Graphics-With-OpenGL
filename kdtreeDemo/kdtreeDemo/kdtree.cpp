#pragma once
#include "kdtree.h"
#include "common.h"

/***************Triangle**************************/
Triangle::Triangle(Point3 &v1, Point3 &v2,Point3 &v3,int id)
{
	setTriangle(v1,v2,v3,id);
}

void Triangle::setTriangle(Point3 &v1, Point3 &v2,Point3 &v3,int id)
{
	m_Vertex[0] = v1;
	m_Vertex[1] = v2;
	m_Vertex[2] = v3;
	normal.x = (m_Vertex[1].y - m_Vertex[0].y)*(m_Vertex[2].z - m_Vertex[0].z) - (m_Vertex[1].z - m_Vertex[0].z)*(m_Vertex[2].y - m_Vertex[0].y);
	normal.x = (m_Vertex[1].z - m_Vertex[0].z)*(m_Vertex[2].x - m_Vertex[0].x) - (m_Vertex[1].x - m_Vertex[0].x)*(m_Vertex[2].z - m_Vertex[0].z);
	normal.x = (m_Vertex[1].x - m_Vertex[0].x)*(m_Vertex[2].y - m_Vertex[0].y) - (m_Vertex[1].y - m_Vertex[0].y)*(m_Vertex[2].x - m_Vertex[0].x);
	m_ObjectID = id;
}
int Triangle::getObjectID()
{
	return m_ObjectID;
}
vertex Triangle::GetVertex(int i)
{
	return m_Vertex[i];
}

void Triangle::Draw()
{
	glBegin(MODEL);
	glColor4f(1.0,0.0,0.0,0.5);
	//glBegin(GL_LINE_LOOP);
	for(int i =0;i<3;i++)
	{
		glNormal3f(normal.x,normal.y,normal.z);
		glVertex3f(m_Vertex[i].x,m_Vertex[i].y,m_Vertex[i].z);
	}
	glEnd();
}

bool Triangle::Intersect(Ray &ray,Point3 &hitPoint)
{
	vector3 e1 = m_Vertex[1] - m_Vertex[0];
	vector3 e2 = m_Vertex[2] - m_Vertex[0];
	vector3 q = ray.direction.cross(e2);
	float a = e1.dot(q);
	if(abs(a)<0.001) 
	{
		hitPoint.set(Point3(0,0,0));
		return false;
	}
	float f = 1/a;
	vector3 s = ray.position - m_Vertex[0];
	float u = f * s.dot(q);
	if(u<0.0)
	{
		hitPoint.set(Point3(0,0,0));
		return false;
	}
	vector3 r = s.cross(e1);
	float v = f * ray.direction.dot(r);
	if(v<0.0||u+v > 1.0)
	{
		hitPoint.set(Point3(0,0,0));
		return false;
	}
	float t = f * e2.dot(r);
	for(int i =0;i<3;i++)
	{
		hitPoint.cell[i] = ray.position.cell[i] + t * ray.direction.cell[i];
	}
	return true;

}

/***************TriangleNode**************************/
void TriangleNode::SetNextNode(TriangleNode* nextNode)
{
	next = nextNode;
}
TriangleNode* TriangleNode::GetNext()
{
	return next;
}
vertex TriangleNode::GetVertex(int i)
{
	return Tri->GetVertex(i);
}
void TriangleNode::SetNodeValue(Point3 &v1, Point3 &v2,Point3 &v3,int id)
{
	Tri=new Triangle(v1,v2,v3,id);
}

/***************TriangleNode**************************/
TriangleList::TriangleList()
{
	m_List = NULL;
	m_Count = 0;
}
int TriangleList::GetCount()
{
	return m_Count;
}
TriangleNode* TriangleList::GetHead()
{
	return m_List;
}
void TriangleList::DeleteHead()
{
	if(m_List)
	{
		m_List = m_List->next;
		m_Count--;
	}
}
void TriangleList::append(TriangleNode* triangle)
{
	if(m_List!=NULL)
	{
		triangle->next = m_List;
		m_List = triangle;
	}else{
		m_List = triangle;
		m_List->next = NULL;
	}
	m_Count++;
}
void TriangleList::Draw()
{
	TriangleNode *templist = m_List;
	while(templist)
	{
		templist->Tri->Draw();
		templist = templist->GetNext();
	}

}

/*****************KDTreeNode**************************/
void KDTreeNode::SetAxis( int Axis)
{
	m_Axis = Axis;
}
int KDTreeNode::GetAxis()
{
	return m_Axis;
}
void KDTreeNode::SetLeft(KDTreeNode* left)
{
	m_lchild = left;
}
KDTreeNode* KDTreeNode::GetLeft()
{
	return m_lchild;
}
void KDTreeNode::SetRight(KDTreeNode* right)
{
	m_rchild = right;
}
KDTreeNode* KDTreeNode::GetRight()
{
	return m_rchild;
}
void KDTreeNode::SetTriangleList(TriangleList *List)
{
	m_List = List;
}
TriangleList* KDTreeNode::GetTriangleList()
{
	return m_List;
}
void KDTreeNode::SetSplitPosition(real split)
{
	m_Split = split;
}
real KDTreeNode::GetSplitPosition()
{
	return m_Split;
}
/*****************SplitList**************************/
SplitList::SplitList()
{
	m_SList = NULL;
}
void SplitList::InsertSplitPos( real splitPos )
{
	SplitNode* entry = new SplitNode();
	entry->next = NULL;
	entry->splitpos	= splitPos;
	entry->nlcount = 0;
	entry->nrcount = 0;
	if(!m_SList)m_SList = entry;else
	{
		if(splitPos < m_SList->splitpos)
		{
			entry->next = m_SList;
			m_SList = entry;
		}
		else if(splitPos == m_SList->splitpos)
		{
			delete entry;
		}else
		{
			SplitNode* list = m_SList;
			while( list->next && (splitPos >= list->next->splitpos))
			{
				if(splitPos == list->next->splitpos)
					delete entry;
				return ;
			};
			entry->next = list->next;
			list->next = entry;
		}
	}
}
SplitNode *SplitList::GetHead()
{
	return m_SList;
}
/*****************Scene**************************/
Scene::Scene()
{
	m_triangles = NULL;

	LoadScene();
	SetTriangle();

	vector3 p1 = vector3( 0, 0, 0 ), p2 = vector3( 10, 10, 10 );
	m_box = aabb( p1, p2 - p1 );
}
void Scene::append()
{

}
aabb Scene::GetBoundingBox()
{
	return m_box;
}
TriangleList* Scene::GetTriangles()
{
	return m_triangles;
}
void Scene::Draw()
{
	m_objects[0].draw();
}
bool Scene::intersectMesh(Ray &ray,Mesh &object)
{
	int numface = object.getnumFaces();

	Point3 v1,v2,v3;
	Point3 hitPoint;
	TriangleNode *triangle = new TriangleNode();
	for(int i =0 ; i< numface; i++)
	{
		int nVerts = object.face[i].nVerts;
		v1 = object.pt[object.face[i].vert[0].vertIndex];
		for(int j=1; j<nVerts-1;j++)
		{
			v2 = object.pt[object.face[i].vert[j].vertIndex];
			v3 = object.pt[object.face[i].vert[j+1].vertIndex];
			triangle->SetNodeValue(v1,v2,v3,0);
			if(triangle->Tri->Intersect(ray,hitPoint))
			{
				delete triangle;
				return true;
			}
			
		}

	}
	return false;
}
void Scene::intersect(Ray &ray)
{
	Point3 hitPoint;
	bool isHit;
	int hitObject[numofObject];
	int index =0;
	for(int k = 0;k<numofObject;k++)
	{
		if(intersectMesh(ray,m_objects[k]))
			hitObject[index++] = k;
	}
	printf("the hit objects without using the kdtree:(no sorting)\n");
	for(int i=0;i<index;i++)
	{
		printf("object sequence(%d):%d\n",i,hitObject[i]);
	}
}

void Scene::LoadScene()
{
	char object[2][10] = {
		"house.txt","tetra.txt"
	};
	float x=0,y=0,z=0;
	int size = 10; // the length of the scene
	for(int i=0;i<numofObject;i++)
	{
		x = rand()% size;
		y = rand()% size;
		z = rand()% size;
		int j = i%2;
		m_objects[i].readFile(object[j]);
		for(int p =0;p<m_objects[i].numVerts;p++)
		{
			m_objects[i].pt[p].x += x;
			m_objects[i].pt[p].y += y;
			m_objects[i].pt[p].z += z;
		}
	}
}
void Scene::SetTriangle()
{
	if(!m_triangles)
		delete []m_triangles;
	m_triangles = new TriangleList();
	for(int k = 0;k<numofObject;k++)
	{
		int numface = m_objects[k].getnumFaces();

		Point3 v1,v2,v3;
		int id=k;
		for(int i =0 ; i< numface; i++)
		{
			int nVerts = m_objects[k].face[i].nVerts;
			v1 = m_objects[k].pt[m_objects[k].face[i].vert[0].vertIndex];
			for(int j=1; j<nVerts-1;j++)
			{
				v2 = m_objects[k].pt[m_objects[k].face[i].vert[j].vertIndex];
				v3 = m_objects[k].pt[m_objects[k].face[i].vert[j+1].vertIndex];
				TriangleNode *triangle = new TriangleNode();
				triangle->SetNodeValue(v1,v2,v3,id);
				m_triangles->append(triangle);
			}

		}
	}
}
/*****************KDTree**************************/
KDTree::KDTree()
{
	m_Root = new KDTreeNode();
	m_Root->Type = LEFT;
}
KDTreeNode* KDTree::GetRoot()
{
	return m_Root;
}
void KDTree::Build(Scene* scene)
{
	TriangleList *triangles= scene->GetTriangles();
	aabb box = scene->GetBoundingBox();
	Subdivide(*m_Root,triangles,box,0);

	//Build the ropes in the kdtree
	KDTreeNode* RS[6];
	for(int i=0; i<6; i++)
		RS[i] = NULL;
	ProcessNode(m_Root,RS);
}

float KDTree::caculateSingleVoxelCost(int numofTriangles,aabb &box)
{
	float area = 2 * (box.w()*box.d() + box.w()*box.h() + box.d()*box.h());
	return area * numofTriangles;
}
float KDTree::caculateCost(SplitNode *split,aabb &frontBox, aabb &backBox)
{
	float frontArea = 2 * (frontBox.w()*frontBox.d() + frontBox.w()*frontBox.h() + frontBox.d()*frontBox.h());
	float backArea = 2 * (backBox.w()*backBox.d() + backBox.w()*backBox.h() +backBox.d()*backBox.h());

	return frontArea * split->nlcount+ backArea * split->nrcount;
}
bool KDTree::findOptimalSplitPositon(TriangleList *list,aabb &box,int depth, real &splitPosition,aabb &frontBox, aabb &backBox)
{
	bool foundOptimalSplit = false;

	//make a list of the split position candidates
	int axis = depth % Dimension;    
	real pos1 = box.GetPos().cell[axis];//the rangle of the axis direction within the box
	real pos2 = box.GetPos().cell[axis] + box.GetSize().cell[axis];

	bool *pright = new bool[list->GetCount()];
	TriangleNode *tempList = list->GetHead();
	SplitList* SList = new SplitList();
	int aidx = 0;
	while(tempList)
	{
		real pos;
		pright[aidx++]= true;
		for(int i=0;i<3;i++)
		{
			pos = tempList->GetVertex(i).cell[axis];
			if((pos >= pos1)&&(pos <= pos2)) SList->InsertSplitPos(pos);
		}
		tempList = tempList->GetNext();
	}

	//determine nlcount / nrcont for each split position
	aabb tempFrontBox = box;
	aabb tempBackBox = box;
	float tempFrontBoxp1 = tempFrontBox.GetPos().cell[axis];
	float tempBackBoxp2 = tempBackBox.GetPos().cell[axis] + tempBackBox.GetSize().cell[axis];
	SplitNode* splist = SList->GetHead();
	while(splist!=NULL)
	{
		tempBackBox.GetPos().cell[axis] = splist->splitpos;
		tempBackBox.GetSize().cell[axis] = pos2 - splist->splitpos;
		tempFrontBox.GetSize().cell[axis] = splist->splitpos - pos1;

		float tempFrontBoxp2 = tempFrontBox.GetPos().cell[axis] + tempFrontBox.GetSize().cell[axis];
		float tempBackBoxp1 = tempBackBox.GetPos().cell[axis];
		tempList = list->GetHead();
		int i = 0;
		while(tempList)
		{
			if(pright[i])
			{
				int position = partitionTriangle(tempList,depth,splist->splitpos);
				switch(position)
				{
				case kdBefore:
					splist->nlcount++;
					pright[i]=false;
					break;
				case kdAfter:
					splist->nrcount++;
					break;
				case kdIntersection:
					splist->nlcount++;
					splist->nrcount++;
					break;
				}
			}else
			{
				splist->nlcount++;
			}
			tempList = tempList->GetNext();
		}
		splist = splist->next;
		i++;
	}
	delete pright;

	float bestPos = 0.0;
	float lowCost = caculateSingleVoxelCost(list->GetCount(),box);
	float bestCost = lowCost;
	splist = SList->GetHead();
	while(splist)
	{
		tempBackBox.GetPos().cell[axis] = splist->splitpos;
		tempBackBox.GetSize().cell[axis] = pos2 - splist->splitpos;
		tempFrontBox.GetSize().cell[axis] = splist->splitpos - pos1;

		real splitcost = cTraversal * lowCost + caculateCost(splist,tempFrontBox,tempBackBox);
		if(splitcost < lowCost)
		{
			foundOptimalSplit = true;
			bestCost = splitcost;
			bestPos = splist->splitpos;
			frontBox = tempFrontBox;
			backBox = tempBackBox;
			
		}
		splist = splist->next;
	}
	splitPosition = bestPos;
	return foundOptimalSplit;
}

/*Function: determines on which side of the node the given triangle belongs
 *Procedure:
 *		It checks for each of the triangle's points on which side of the split
 *		plane it is, and counts how many points it finds on the front and back 
 *		sides.
 *		If there are points on one side, and no points on the other side, then
 *		the one side is the result of the function.
 *		If there are points on both sides, then the triangle intersects the split
 *		plane. And if there are no points on either side, then the triangle intersects
 *		the split plane as well.
 *Date:11-30-2012 wait_in_2010
*/
int KDTree::partitionTriangle(TriangleNode *triangle,int depth,int splitPosition)
{
	int numberofBefore = 0;
	int numberofAfter = 0;
	int axis = depth % Dimension;
	switch(axis)
	{
	case kdPlaneXY:
		for(int i =0; i<3;i++)
		{
			vertex point = triangle->GetVertex(i);
			if(point.z < splitPosition)
			{
				numberofBefore++;
			}else if(point.z > splitPosition)
			{
				numberofAfter++;
			}
		}
		break;
	case kdPlaneXZ:
		for(int i =0; i<3;i++)
		{
			vertex point = triangle->GetVertex(i);
			if(point.y < splitPosition)
			{
				numberofBefore++;
			}else if(point.y > splitPosition)
			{
				numberofAfter++;
			}
		}
		break;
	case kdPlaneYZ:
		for(int i =0; i<3;i++)
		{
			vertex point = triangle->GetVertex(i);
			if(point.x < splitPosition)
			{
				numberofBefore++;
			}else if(point.x > splitPosition)
			{
				numberofAfter++;
			}
		}
		break;
	}
	if(numberofBefore>0)
	{
		if(numberofAfter>0)
		{
			return kdIntersection;
		}else
		{
			return kdBefore;
		}
	}else
	{
		if(numberofAfter>0)
		{
			return kdAfter;
		}else
		{
			return kdIntersection;
		}
	}
}

void KDTree::Subdivide(KDTreeNode &node,TriangleList *triangles,aabb& box,int depth)
{
	if(!triangles)return ;
	int count = triangles->GetCount();
	if( count < minTrianglesPerleafNode ||(depth>=20&&count<=maxTrianglesPerleafNode))
	{
		node.SetTriangleList(triangles);
		node.Type = LEFT;
		node.box = box;
		return ;
	}

	
	aabb frontBox,backBox;
	real splitPosition;
	bool foundOptimalSplit = findOptimalSplitPositon(triangles,box,depth,splitPosition,frontBox, backBox);

	if(foundOptimalSplit == true)
	{
		
		node.Type = PATITION;
		node.SetAxis(depth%Dimension);
		node.SetSplitPosition(splitPosition);
		node.box = box;
		

		TriangleList *frontTriangles = new TriangleList();
		TriangleList *backTriangles =	new TriangleList();
		for( int i=0; i < count; i++)
		{
			//Determine on which side of the split each
			//triangle belongs.
		    TriangleNode * triangle= triangles->GetHead();
			triangles->DeleteHead();
			
			int position = partitionTriangle(triangle,depth,splitPosition);
			triangle->next = NULL;
			switch(position)
			{
			case kdBefore:
				frontTriangles->append(triangle);
				break;
			case kdAfter:
				backTriangles->append(triangle);
				break;
			case kdIntersection:
				//frontTriangles->append(triangle);
				backTriangles->append(triangle);
				break;
			}
			
		}
		node.m_lchild = new KDTreeNode();
		node.m_rchild = new KDTreeNode();
		Subdivide(*node.m_lchild,frontTriangles,frontBox,depth+1);
		Subdivide(*node.m_rchild,backTriangles,backBox,depth+1);
	}else
	{
		
		node.SetTriangleList(triangles);
		node.Type = LEFT;
		node.box = box;
	}

}
void  KDTree::Draw(KDTreeNode* templist)
{
		if(templist->Type!=LEFT)
		{
			DrawSplitPlane(templist);
			Draw(templist->GetLeft());
			Draw(templist->GetRight());
		}else
		{
			templist->m_List->Draw();
		}
}

void  KDTree::DrawSplitPlane(KDTreeNode *templist)
{
		Point3 center;
		/*printf("%f\n",templist->m_Split);
		printf("%f %f %f\n",templist->box.x(),templist->box.y(),templist->box.z());
		printf("%f %f %f\n",templist->box.GetSize().x,templist->box.GetSize().y,templist->box.GetSize().z);*/
		center.x = (2*templist->box.GetPos().x + templist->box.GetSize().x)/2;
		center.y = (2*templist->box.GetPos().y + templist->box.GetSize().y)/2;
		center.z = (2*templist->box.GetPos().z + templist->box.GetSize().z)/2;
		if(templist->m_Axis==0)
		{
				center.x = templist->m_Split;
				glPushMatrix();
				GLfloat mat_diffuse[] = {0.0f,1.0f,0.0f,0.2f};
				glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
				glNormal3f(1.0,0.0,0.0);
				glBegin(MODEL);
					glVertex3f(center.x,center.y-templist->box.h()/2,center.z-templist->box.d()/2);
					glVertex3f(center.x,center.y+templist->box.h()/2,center.z-templist->box.d()/2);
					glVertex3f(center.x,center.y+templist->box.h()/2,center.z+templist->box.d()/2);
					glVertex3f(center.x,center.y-templist->box.h()/2,center.z+templist->box.d()/2);
				glEnd();
				glPopMatrix();

		}
		else if(templist->m_Axis==1)

			{
				center.y = templist->m_Split;
				glPushMatrix();
				GLfloat mat_diffuse[] = {1.0f,0.0f,0.0f,0.2f};
			    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
				glNormal3f(0.0,1.0,0.0);
				glBegin(MODEL);
				glVertex3f(center.x-templist->box.w()/2,center.y,center.z-templist->box.d()/2);
				glVertex3f(center.x+templist->box.w()/2,center.y,center.z-templist->box.d()/2);
				glVertex3f(center.x+templist->box.w()/2,center.y,center.z+templist->box.d()/2);
				glVertex3f(center.x-templist->box.w()/2,center.y,center.z+templist->box.d()/2);
				glEnd();
				glPopMatrix();
		}
		else
			{
				center.z =templist->m_Split;
				glPushMatrix();
				GLfloat mat_diffuse[] = {0.0f,0.0f,1.0f,0.2f};
				glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
				glNormal3f(0.0,0.0,1.0);
				glBegin(MODEL);
				glVertex3f(center.x-templist->box.w()/2,center.y-templist->box.h()/2,center.z);
				glVertex3f(center.x+templist->box.w()/2,center.y-templist->box.h()/2,center.z);
				glVertex3f(center.x+templist->box.w()/2,center.y+templist->box.h()/2,center.z);
				glVertex3f(center.x-templist->box.w()/2,center.y+templist->box.h()/2,center.z);
				glEnd();
				glPopMatrix();
		}
		
			
		
}

void KDTree::Optimize(KDTreeNode* R,Side s,aabb AABB)
{
	if(!R) return;
	while(R->Type != LEFT)
	{
		if(s-2*R->m_Axis>=0&&s-2*R->m_Axis<=1)
		{
			if(s%2==0)
				R = R->m_rchild;
			else 
				R = R->m_lchild;

		}

		else if(R->m_Split >=AABB.GetPos().cell[R->m_Axis]+AABB.GetSize().cell[R->m_Axis])
			R = R->m_lchild;

		else if(R->m_Split <= AABB.GetPos().cell[R->m_Axis])
			R = R->m_rchild;

		else
			break;
	}
}

void KDTree::ProcessNode(KDTreeNode* N,KDTreeNode *RS[])
{
	if(N->Type==LEFT)
	{
		for(int i=0;i<6;i++)
			N->ropes[i] = RS[i];
		return ;
	}
	for(int i=0; i< 6; i++)
	{
		Optimize(RS[i],(Side)i,N->box);
	}
	Side  SL=Left,SR = Right;
	if(N->m_Axis==1)
	{
		SL = Bottom;
		SR =Top;
	}else if(N->m_Axis==2)
	{
		SL = Back;
		SR = Front;
	}

	float V = N->m_Split;

	KDTreeNode* RSL[6];
	for(int i=0;i<6;i++)
		RSL[i] = RS[i];
	RSL[SR] = N->m_rchild;
	ProcessNode(N->m_lchild,RSL);

	KDTreeNode* RSR[6];
	for(int i=0; i<6; i++)
		RSR[i] = RS[i];
	RSR[SL] = N->m_lchild;
	ProcessNode(N->m_rchild,RSR);
}

int index[6][4] = {
	
	0 , 3 , 7 , 4 ,//left
	1 , 2 , 6 , 5 ,//right

	4 , 5 , 6 , 7 ,//top
	0 , 1 , 2 , 3 ,//bottom
	
	2 , 3 , 7 , 6 ,//front
	0 , 1 , 5 , 4 //back

};
KDTreeNode* KDTree::findLeaf(KDTreeNode* N,Point3 pos)
{
	while(N->Type!=LEFT)
	{
			if(pos.cell[N->m_Axis]>N->m_Split)
				N = N->m_rchild;
			else 
				N = N->m_lchild;
	}
	return N;
}

int KDTree::Intersect(TriangleNode* head,Ray &ray)
{
	Point3 hitPoint;
	int count = 0;
	static int k = 0;
	while(head)
	{
		if(head->Tri->Intersect(ray,hitPoint))
		{
			count++;
			if(k!=head->Tri->getObjectID())
			{
				printf("the hit objects:%d\n",head->Tri->getObjectID());
				k = head->Tri->getObjectID();
			}
		}
		head = head->next;
	}
	
	return count;
}

void KDTree::GetBoxVertex(aabb box, Point3 *Pos)
{
	Point3 start;
	Point3 end;
	for(int i = 0; i< 3; i++)
	{
		start.cell[i] = box.GetPos().cell[i];
		end.cell[i] = box.GetPos().cell[i] + box.GetSize().cell[i];
	}

	Pos[0].set(start);
	Pos[6].set(end);

	Pos[1].set(Point3(end.x,start.y,start.z));
	Pos[2].set(Point3(end.x,start.y,end.z));
	Pos[3].set(Point3(start.x,start.y,end.z));

	Pos[4].set(Point3(start.x,end.y,start.z));
	Pos[5].set(Point3(end.x,end.y,start.z));
	Pos[7].set(Point3(start.x,end.y,end.z));
}
bool IsNotEqual(Point3 ray_Pos,Point3 hitPoint)
{
	for(int i =0;i<3;i++)
		if(abs(ray_Pos.cell[i]-hitPoint.cell[i])>0.001)
			return true;
	return false;
}
bool KDTree::IntersectPlane(Ray &ray,Point3 ray_Pos,Point3 p0,Point3 p1,Point3 p2,Point3 p3,Point3 &hitPoint)
{
	Triangle* tri = new Triangle();
	tri->setTriangle(p0,p1,p2,0);
	if(tri->Intersect(ray,hitPoint))
		if(IsNotEqual(ray_Pos,hitPoint))	
		    return true;
	tri->setTriangle(p0,p2,p3,0);
	if(tri->Intersect(ray,hitPoint))
		if(IsNotEqual(ray_Pos,hitPoint))
		    return true;
	return false;
}
void KDTree::TraversalRope(KDTreeNode *N, Ray &ray,Point3 ray_Pos)
{
	if(N==NULL)
		return ;
	N = findLeaf(N,ray_Pos);
	Intersect(N->m_List->GetHead(),ray);
	Point3 Pos[8];
	Point3 hitPoint;
	int i;
	GetBoxVertex(N->box,Pos);

	for(i=0;i<6;i++)
		if(IntersectPlane(ray,ray_Pos,Pos[index[i][0]],Pos[index[i][1]],Pos[index[i][2]],Pos[index[i][3]],hitPoint))
			break;
	
	if(i>5)
		i++;
	ray_Pos = hitPoint;
	N = N->ropes[i];
	TraversalRope(N,ray,ray_Pos);
}