//SDL.cpp  , fsh, 11/4/99
// support code for the classes in SDL.h

#include "SDL_arch.h"
#include "RGBpixmap.h"

// Vector3 methods
Vector3 Vector3 :: cross(Vector3 b) //return this cross b
{
	Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	return c;
}
float Vector3 :: dot(Vector3 b) // return this dotted with b
{return x * b.x + y * b.y + z * b.z;}

//******************************************************************
void Vector3 :: normalize()//adjust this vector to unit length
{		
	double sizeSq = x * x + y * y + z * z;
	if(sizeSq < 0.0000001)
	{
		cerr << "\nnormalize() sees vector (0,0,0)!";
		return; // does nothing to zero vectors;
	}
	float scaleFactor = 1.0/(float)sqrt(sizeSq);
	x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
}

//******************************************************************
// Color3 methods
void Color3 ::add(Color3& src, Color3& refl)
{ // add the product of source color and reflection coefficient
	red   += src.red   * refl.red;
	green += src.green * refl.green; 
	blue  += src.blue  * refl.blue;
}

//******************************************************************
void Color3:: add(Color3& colr)
{ 
	// add colr to this color
	red += colr.red ; green += colr.green; blue += colr.blue;
}

//******************************************************************
void Color3 :: build4tuple(float v[])
{
	// load 4-tuple with this color: v[3] = 1 for homogeneous
	v[0] = red; v[1] = green; v[2] = blue; v[3] = 1.0f;
}


//Affine4 methods
//******************************************************************
Affine4::Affine4(){ // make identity transform
	m[0] = m[5]  = m[10] = m[15] = 1.0;
	m[1] = m[2]  = m[3]  = m[4]  = 0.0;
	m[6] = m[7]  = m[8]  = m[9]  = 0.0;
	m[11]= m[12] = m[13] = m[14] = 0.0;
}

//******************************************************************
void Affine4 :: setIdentityMatrix(){ // make identity transform
	m[0] = m[5]  = m[10] = m[15] = 1.0;
	m[1] = m[2]  = m[3]  = m[4]  = 0.0;
	m[6] = m[7]  = m[8]  = m[9]  = 0.0;
	m[11]= m[12] = m[13] = m[14] = 0.0;
}

//******************************************************************
void Affine4 ::set(Affine4 a)// set this matrix to a
{
	for(int i = 0; i < 16; i++)
		m[i]=a.m[i];
}

//<<<<<<<<<<<<<< preMult >>>>>>>>>>>>
void Affine4 ::preMult(Affine4 n)
{// postmultiplies this with n
	float sum;
	Affine4 tmp; 
	tmp.set(*this); // tmp copy
	// following mult's : this = tmp * n
	for(int c = 0; c < 4; c++)
		for(int r = 0; r <4 ; r++)
		{
			sum = 0;
			for(int k = 0; k < 4; k++)
				sum += n.m[4 * k + r]* tmp.m[4 * c + k];
			m[4 * c + r] = sum;
		}// end of for loops
}// end of preMult()	

//<<<<<<<<<<<< postMult >>>>>>>>>>>
void Affine4 ::postMult(Affine4 n){// postmultiplies this with n
	float sum;
	Affine4 tmp; 
	tmp.set(*this); // tmp copy
	for(int c = 0; c < 4; c++)// form this = tmp * n
		for(int r = 0; r <4 ; r++)
		{
			sum = 0;
			for(int k = 0; k < 4; k++)
				sum += tmp.m[4 * k + r]* n.m[4 * c + k];
			m[4 * c + r] = sum;
		}// end of for loops
}

// AffineStack methods
//******************************************************************
void AffineStack :: dup()
{
	AffineNode* tmp = new AffineNode;			
	tmp->affn = new Affine4(*(tos->affn));
	tmp->invAffn = new Affine4(*(tos->invAffn));
	tmp->next = tos;
	tos = tmp;
}

//******************************************************************
void AffineStack :: setIdentity() // make top item the identity matrix
{ 
	assert(tos != NULL);
	tos->affn->setIdentityMatrix();
	tos->invAffn->setIdentityMatrix();
}

//******************************************************************
void AffineStack :: popAndDrop()
{
	if(tos == NULL) return; // do nothing
	AffineNode *tmp = tos;
	tos = tos->next;
	delete tmp; // should call destructor, which deletes trices
}	

//******************************************************************
void AffineStack :: releaseAffines()
{ // pop and drop all remaining items 
	while(tos) popAndDrop();
}

//******************************************************************
void AffineStack :: rotate(float angle, Vector3 u)
{
	Affine4 rm; // make identity matrix
	Affine4 invRm;
	u.normalize(); // make the rotation axis unit length
	float ang = angle * 3.14159265/ 180; // deg to  
	float c = cos(ang), s = sin(ang);
	float mc = 1.0 - c;
	 	//fill the 3x3 upper left matrix - Chap.5 p. 29
	rm.m[0] = c + mc * u.x * u.x;
	rm.m[1] = mc * u.x * u.y + s * u.z;
	rm.m[2] = mc * u.x * u.z - s * u.y;
	rm.m[4] = mc * u.y * u.x - s * u.z;
	rm.m[5] = c + mc * u.y * u.y;
	rm.m[6] = mc * u.y * u.z + s * u.x;
	rm.m[8] = mc * u.z * u.x + s * u.y;
	rm.m[9] = mc * u.z * u.y - s * u.x;
	rm.m[10] = c + mc * u.z * u.z;
	// same for inverse : just sign of s is changed
	invRm.m[0] = c + mc * u.x * u.x;
	invRm.m[1] = mc * u.x * u.y - s * u.z;
	invRm.m[2] = mc * u.x * u.z + s * u.y;
	invRm.m[4] = mc * u.y * u.x + s * u.z;
	invRm.m[5] = c + mc * u.y * u.y;
	invRm.m[6] = mc * u.y * u.z - s * u.x;
	invRm.m[8] = mc * u.z * u.x - s * u.y;
	invRm.m[9] = mc * u.z * u.y + s * u.x;
	invRm.m[10] = c + mc * u.z * u.z;
	tos->affn->postMult(rm);
	tos->invAffn->preMult(invRm);
}

//******************************************************************
void AffineStack :: scale(float sx, float sy, float sz)
{ 
	// post-multiply top item by scaling
#define sEps 0.00001
	Affine4 scl;// make an identity
	Affine4 invScl;
	scl.m[0]  = sx; 
	scl.m[5]  = sy; 
	scl.m[10] = sz;// adjust it to a scaling matrix
	if(fabs(sx) < sEps || fabs(sy) < sEps || fabs(sz) < sEps)
	{
		cerr << "degenerate scaling transformation!\n";
	}
	invScl.m[0]  = 1/sx; invScl.m[5]  = 1/sy; invScl.m[10] = 1/sz;
	tos->affn->postMult(scl); //
	tos->invAffn->preMult(invScl);
}		

//******************************************************************
void AffineStack :: translate(Vector3 d)
{
	Affine4 tr; // make identity matrix
	Affine4 invTr;
	tr.m[12] = d.x; tr.m[13] = d.y;	tr.m[14] = d.z;
	invTr.m[12] = -d.x;	invTr.m[13] = -d.y; invTr.m[14] = -d.z;
	tos->affn->postMult(tr);
	tos->invAffn->preMult(invTr);
}

// Material methods
//******************************************************************
void Material :: setDefault(){
	textureType = 0; // for none
	numParams = 0;
	reflectivity = transparency = 0.0;
	speedOfLight = specularExponent = 1.0;
	specularFraction = 0.0;
	surfaceRoughness = 1.0;
	ambient.set(0.1f,0.1f,0.1f);
	diffuse.set(0.8f,0.8f,0.8f);
	specular.set(0,0,0);
	emissive.set(0,0,0);
}

//******************************************************************
void Material :: set(Material& m)
{
	textureType = m.textureType;
	numParams = m.numParams;
	for(int i = 0; i < numParams; i++) params[i] = m.params[i];
	transparency = m.transparency;
	speedOfLight = m.speedOfLight;
	reflectivity = m.reflectivity;
	specularExponent = m.specularExponent;
	specularFraction = m.specularFraction;
	surfaceRoughness = m.surfaceRoughness;
	ambient.set(m.ambient);
	diffuse.set(m.diffuse);
	specular.set(m.specular);
	emissive.set(m.emissive);
}


// Shape methods
//******************************************************************
void Shape :: tellMaterialsGL()
{
	float amb[4],diff[4],spec[4], emiss[4];
	float zero[] = {0,0,0,1};
	mtrl.ambient.build4tuple(amb); // fill the array
	mtrl.diffuse.build4tuple(diff);
	mtrl.specular.build4tuple(spec);
	mtrl.emissive.build4tuple(emiss);
	glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_AMBIENT,amb);
	glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_DIFFUSE,diff);
			glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_SPECULAR,spec);
			glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_EMISSION,emiss);
			glMaterialf(GL_FRONT/*_AND_BACK*/,GL_SHININESS,mtrl.specularExponent);
}




//Mesh methods
//******************************************************************
Mesh :: Mesh(){
			numVerts = numFaces = numNorms = 0; 
			pt = NULL; norm  =  NULL; face = NULL;
			lastVertUsed = lastNormUsed = lastFaceUsed = -1;
}

//******************************************************************
void Mesh :: freeMesh()
{ // free up memory used by this mesh.
			delete [] pt; // release whole vertex list
			delete [] norm;
			for(int f = 0; f < numFaces; f++)
				delete[] face[f].vert; // delete the vert[] array of this face
			delete [] face;
}

//******************************************************************
int Mesh :: isEmpty() 
{
	return (numVerts == 0) || (numFaces == 0) || (numNorms == 0);
}

//******************************************************************
void Mesh :: makeEmpty() 
{ 
	numVerts = numFaces = numNorms = 0;
}

//******************************************************************
void Mesh :: drawOpenGL()
{ 
			tellMaterialsGL(); 	glPushMatrix();
			glMultMatrixf(transf.m); 
			drawMesh();	
			//if(doEdges) drawEdges();
			glPopMatrix();
} 

//******************************************************************
Mesh :: Mesh(string fname){ // read this file to build mesh
			numVerts = numFaces = numNorms = 0; 
			pt = NULL; norm  =  NULL; face = NULL;
			lastVertUsed = lastNormUsed = lastFaceUsed = -1;
			readMesh(fname);
}

//******************************************************************
Vector3 Mesh :: newell4(int indx[])
{ /* return the normalized normal to face with vertices
  pt[indx[0]],...,pt[indx[3]]. i.e. indx[] contains the four indices 
	into the vertex list to be used in the Newell calculation */
			Vector3 m;
			for(int i = 0; i < 4 ; i++)
			{
				int next = (i== 3) ? 0 : i + 1; // which index is next?
				int f = indx[i], n = indx[next]; // names for the indices in the pair
				m.x += (pt[f].y - pt[n].y) * (pt[f].z + pt[n].z);
				m.y += (pt[f].z - pt[n].z) * (pt[f].x + pt[n].x);
				m.z += (pt[f].x - pt[n].x) * (pt[f].y + pt[n].y);
			}
			m.normalize();
			return m;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<< readMesh >>>>>>>>>>>>>>>>>>>>>>>>
void Mesh:: readMesh(string fname)
{
	fstream inStream;
	inStream.open(fname.c_str(), ios ::in); //open needs a c-like string
	if(inStream.fail() || inStream.eof()) 
	{
		cout << "can't open file or eof: " << fname << endl; 
		makeEmpty();
		return;
	}
	inStream >> numVerts >> numNorms >> numFaces;
	// make arrays for vertices, normals, and faces
	pt = new Point3[numVerts];        
	//assert(pt != NULL);

	norm = new Vector3[numNorms];     
	//assert(norm != NULL);

	face = new Face[numFaces];        
	assert(face != NULL);

	for(int i = 0; i < numVerts; i++) 	// read in the vertices
		inStream >> pt[i].x >> pt[i].y >> pt[i].z;
	for(int ii = 0; ii < numNorms; ii++) 	// read in the normals
		inStream >> norm[ii].x >> norm[ii].y >> norm[ii].z;
	for(int f = 0; f < numFaces; f++)   // read in face data
	{
		inStream >> face[f].nVerts;
		int n = face[f].nVerts;
		face[f].vert = new VertexID[n]; assert(face[f].vert != NULL);
		for(int k = 0; k < n; k++) 		// read vertex indices for this face
			inStream >> face[f].vert[k].vertIndex;
		for(int kk = 0; kk < n; kk++) 		// read normal indices for this face
			inStream >> face[f].vert[kk].normIndex;
	}
	inStream.close();
} // end of readMesh

/*
//<<<<<<<<<<<<<<<<<<<<<< drawMesh >>>>>>>>>>>>>>>>>>>>
void Mesh :: drawMesh()
{ 
	// draw each face of this mesh using OpenGL: draw each polygon.
	if(isEmpty()) 
	{	
		cout << "From inside drawMesh, the mesh is empty\n";
		return; // mesh is empty
	}

	for(int f = 0; f < numFaces; f++)
	{
		int n = face[f].nVerts;
		glBegin(GL_POLYGON);
		for(int v = 0; v < n; v++)
		{
			int in = face[f].vert[v].normIndex;
			assert(in >= 0 && in < numNorms);
			glNormal3f(norm[in].x, norm[in].y, norm[in].z);
			int iv = face[f].vert[v].vertIndex; assert(iv >= 0 && iv < numVerts);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();	
	}
	glFlush();
}	
*/


// draw mesh including textures
//<<<<<<<<<<<<<<<<<<<<<< drawMesh >>>>>>>>>>>>>>>>>>>>
void Mesh :: drawMesh()
{ 
	// draw each face of this mesh using OpenGL: draw each polygon.
    if(isEmpty()) 
		return; // mesh is empty

    for(int f = 0; f < numFaces; f++)
    {
        int n = face[f].nVerts;
        if(face[f].whichTxtr > 0) //enable texturing and bind proper texture to this polygon
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,face[f].whichTxtr);

            glBegin(GL_POLYGON);
            for(int v = 0; v < n; v++) // for each vertex
            {
                int in = face[f].vert[v].normIndex; // get normal index
                   assert(in >= 0 && in < numNorms);
                glNormal3f(norm[in].x, norm[in].y, norm[in].z); // set normal to vertex
				
				int it = face[f].vert[v].txtrIndex; // get texture coord index for this vertex
                glTexCoord2f(txtrVert[it].x , txtrVert[it].y); // set texture coords
				
				int iv = face[f].vert[v].vertIndex; // get vertex index
                assert(iv >= 0 && iv < numVerts);
                glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
            }
            glEnd();
        }
        else // texture not used for this face - normal rendering as before
        {
            glDisable(GL_TEXTURE_2D);
            glBegin(GL_POLYGON);
            for(int v = 0; v < n; v++)
            {
                int in = face[f].vert[v].normIndex;
                   assert(in >= 0 && in < numNorms);
                glNormal3f(norm[in].x, norm[in].y, norm[in].z);
                int iv = face[f].vert[v].vertIndex; 
                   assert(iv >= 0 && iv < numVerts);
                glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
            }
            glEnd();
        }
    }
    glFlush();
}             	


//<<<<<<<<<<<<<<<<<<<<<<<<<<<< write mesh>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void Mesh:: writeMesh(char * fname)
{ // write this mesh object into a new Chapter 6 format file.
	if(numVerts == 0 || numNorms   == 0 || numFaces == 0) return; //empty
	fstream outStream(fname, ios ::out); // open the output stream
	if(outStream.fail()) {cout << "can't make new file: " << fname << endl; 
	return;}
	outStream << numVerts << " " << numNorms << " " << numFaces << "\n";
	// write the vertex and vertex normal list
	for(int i = 0; i < numVerts; i++)
		outStream << pt[i].x   << " " << pt[i].y   << " " << pt[i].z << "\n";
	for(int ii = 0; ii < numNorms; ii++)
		outStream  << norm[ii].x << " " << norm[ii].y << " " << norm[ii].z << "\n";
	// write the face data
	for(int f = 0; f < numFaces; f++)
	{
		int n = face[f].nVerts;
		outStream << n << "\n";
		for(int v = 0; v < n; v++)// write vertex indices for this face
			outStream << face[f].vert[v].vertIndex << " ";	outStream << "\n";
		for(int k = 0; k < n; k++)	// write normal indices for this face 
			outStream << face[f].vert[k].normIndex << " "; outStream << "\n";
	}
	outStream.close();
}

//Extrude methods
//******************************************************************
Extrude::Extrude()
{
	numPts = 0; 
	ptArr = NULL;
	upVector = Vector3( 0, 0, 3 );
	myMesh = new Mesh();
}

//******************************************************************
Extrude::Extrude( string fname )
{
	numPts = 0; 
	ptArr = NULL;
	upVector = Vector3( 0, 0, 3 );
	readExtrude( fname );
	myMesh = new Mesh();
}

//******************************************************************
void Extrude::readExtrude( string fname )
{
	fstream inStream;
	inStream.open(fname.c_str(), ios ::in); //open needs a c-like string

	if(inStream.fail() || inStream.eof()) 
	{
		cout << "can't open file or eof: " << fname << endl; 
		return;
	}
	
	inStream >> numPts;		// read it in

	ptArr = new Point2[numPts];
	int sizeArr = numPts;

	//assert(pt != NULL);
	for(int i = 0; i < numPts; i++) 	// read in the vertices
		inStream >> ptArr[i].x >> ptArr[i].y ;

}

//******************************************************************
void Extrude::makeExtrudedQuadStrip()
{
	myMesh->makeExtrudedQuadStrip( ptArr, numPts, upVector );
	myMesh->drawMesh();
}

//******************************************************************
void Extrude::drawOpenGL()
{
		tellMaterialsGL(); 	glPushMatrix();
		glMultMatrixf(transf.m); 
		//drawMesh();
		makeExtrudedQuadStrip();
		//if(doEdges) drawEdges();
		glPopMatrix();
}

//<<<<<<<<<<<<<<<<<<<<< SIZESQ >>>>>>>>>>>>>>>>
#define SIZESQ(n) ((n.x)*(n.x)+(n.y)*(n.y)+(n.z)*(n.z))

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< newell4Pts >>>>>>>>>>>>>>>>>>
Vector3 newell4Pts(Point3 a, Point3 b, Point3 c, Point3 d)
{ // don't normalize the vector here
	Vector3 m;
	m.x = (a.y-b.y)*(a.z+b.z)+(b.y-c.y)*(b.z+c.z)+(c.y-d.y)*(c.z+d.z)+(d.y-a.y)*(d.z+a.z);
	m.y = (a.z-b.z)*(a.x+b.x)+(b.z-c.z)*(b.x+c.x)+(c.z-d.z)*(c.x+d.x)+(d.z-a.z)*(d.x+a.x);
	m.z = (a.x-b.x)*(a.y+b.y)+(b.x-c.x)*(b.y+c.y)+(c.x-d.x)*(c.y+d.y)+(d.x-a.x)*(d.y+a.y);
	//cout << " in newell4Pts, sizeSq(m) = " << SIZESQ(m) << endl;
	return m;
}


//<<<<<<<<<<<<<<<<<<<<<< makeExtruded QuadStripMesh >>>>>>>>>>>>>>>
void Mesh:: makeExtrudedQuadStrip(Point2 P[], int numPts, Vector3 w)
{
	
	// quad strip in list P[0],P[1], P[2], .., of points in xy-plane
	// extrusion is along vector w
	// Original quadstrip should be ordered CW: P0,P1,P3,P2 
	// as seen from pos. z-axis looking at xy-plane.
	int numSegments = numPts/2 - 1;
	numVerts = numPts * 2; // for all waists = quadrilaterals

	//numFaces = numSegments * 4; // each segment has 4 faces
	numFaces = numSegments * 4 + 2; // each segment has 4 faces
	

	numNorms = numFaces; // for visibly flat faces

	pt   = new Point3[numVerts];  assert(pt); // make space for the lists
	face = new Face[numFaces];    assert(face);
	norm = new Vector3[numNorms]; assert(norm);

	for(int s = 0; s <= numSegments; s++) // for each waist
	//for(int s = 0; s < numSegments; s++) // for each waist
	{
			int twoI = 2 * s;
			pt[4*s    ].set(P[twoI    ].x,P[twoI    ].y,0);
			pt[4*s + 1].set(P[twoI + 1].x,P[twoI + 1].y,0);
			pt[4*s + 2].set(P[twoI + 1].x + w.x,P[twoI + 1].y + w.y, w.z);
			pt[4*s + 3].set(P[twoI    ].x + w.x,P[twoI    ].y + w.y,w.z);
	}

	int which;
	for(int i = 0; i < numSegments; i++)
		for(int j = 0; j < 4; j++)
		{
			//int which = 4 * i + j; // which face
			which = 4 * i + j; // which face

			int i0 = which, i1 = i0 + 4,  i3 = 4 * i + (j + 3)%4,  i2 = i3 + 4;

			// added to make newell4Pts function work
			//int index[4] = { pt[i0],pt[i1],pt[i2], pt[i3] };

			norm[which] = newell4Pts( pt[i0], pt[i1], pt[i2], pt[i3] );
			//norm[which] = newell4Pts( index );

			norm[which].normalize();
			face[which].vert = new VertexID[4]; assert(face[which].vert != NULL);
			face[which].nVerts = 4;
			face[which].vert[0].vertIndex = i0;// same as norm index
			face[which].vert[0].normIndex = which;
			face[which].vert[1].vertIndex = i1; 
			face[which].vert[1].normIndex = which;
			face[which].vert[2].vertIndex = i2;
			face[which].vert[2].normIndex = which;
			face[which].vert[3].vertIndex = i3;
			face[which].vert[3].normIndex = which;
		}

		// added for the top and bottom faces
		norm[which+1] = Vector3( 0.0, 0.0, -1.0 );

		norm[which+1].normalize();
		face[which+1].vert = new VertexID[4]; assert(face[which+1].vert != NULL);
		face[which+1].nVerts = 4;

		face[which+1].vert[0].vertIndex = 0;
		face[which+1].vert[0].normIndex = which+1;
		face[which+1].vert[1].vertIndex = 1;
		face[which+1].vert[1].normIndex = which+1;
		face[which+1].vert[2].vertIndex = 2;
		face[which+1].vert[2].normIndex = which+1;
		face[which+1].vert[3].vertIndex = 3;
		face[which+1].vert[3].normIndex = which+1;

		
		norm[which+2] = Vector3( 0.0, 0.0, 1.0 );

  		norm[which+2].normalize();
		face[which+2].vert = new VertexID[4]; assert(face[which+1].vert != NULL);
		face[which+2].nVerts = 4;

		int topIndex = numSegments * 4;
		face[which+2].vert[0].vertIndex = topIndex;
		face[which+2].vert[0].normIndex = which+2;
		face[which+2].vert[1].vertIndex = topIndex+1;
		face[which+2].vert[1].normIndex = which+2;
		face[which+2].vert[2].vertIndex = topIndex+2;
		face[which+2].vert[2].normIndex = which+2;
		face[which+2].vert[3].vertIndex = topIndex+3;
		face[which+2].vert[3].normIndex = which+2;
		

		
} // end, makeExtrudedQuadStrip


//<<<<<<<<<<<<<<<<<<<<<< makeExtruded QuadStripMesh >>>>>>>>>>>>>>>
void Mesh:: makeExtrudedQuadStrip(Point2 P[], int numPts, Vector3 w, char* fname)
{
		// quad strip in list P[0],P[1], P[2], .., of points in xy-plane
        
		// extrusion is along vector w
        // Original quadstrip should be ordered CW: P0,P1,P3,P2 
        // as seen from pos. z-axis looking at xy-plane
        int numSegments = numPts/2 - 1;

        unsigned int ID = 201; // my ad hoc number for this texture

        numVerts = numPts * 2;			// for all waists = quadrilaterals
        numFaces = numSegments * 4;		// each segment has 4 faces
        numNorms = numFaces;			// for visibly flat faces
        RGBpixmap pix;
        pix.readBMPFile(fname); // read .bmp file to make pixmap
        pix.setTexture(ID);

        //cout << " numVerts, Norms, Faces = " << numVerts << "," << numNorms << "," << numFaces << endl;
   
		pt   = new Point3[numVerts];  assert(pt); // make space for the lists
        face = new Face[numFaces];    assert(face);
        norm = new Vector3[numNorms]; assert(norm);

        numTxtrs = 1;  // texture on top faces only
        numTxtrVerts = numPts; // one texture coord pair for quadstrip vertex
        txtrVert = new Point2[numTxtrVerts]; assert(txtrVert != NULL); 

        float left = P[0].x, bott = P[0].y, right = P[0].x, top = P[0].y; //search for bounding box
        for(int j = 1; j < numPts; j++)
        {
                if(P[j].x < left) left = P[j].x;
                if(P[j].x > right) right = P[j].x;
                if(P[j].y < bott) bott = P[j].y;
                if(P[j].y > top) top = P[j].y;
        }

        float wid = right - left, ht = top - bott; // dimensions of bounding box

		//float numTilesHoriz = 13, numTilesVert = 4; // change this for your Mesh !!

		// I changed it to one 
		float numTilesHoriz = 1, numTilesVert = 1; // change this for your Mesh !!

        for(int s = 0; s <= numSegments; s++) // for each waist
        {
			int twoI = 2 * s;
			Point2 p1,p2;
			p1.set(P[twoI].x,P[twoI].y);
			p2.set(P[twoI + 1].x,P[twoI + 1].y);
			pt[4*s    ].set(p1.x,p1.y,0);
			pt[4*s + 1].set(p2.x,p2.y,0);
			pt[4*s + 2].set(p2.x + w.x,p2.y + w.y, w.z);
			pt[4*s + 3].set(p1.x + w.x,p1.y + w.y,w.z);

			Point2 q1,q2;

			q1.set((p1.x-left)*numTilesHoriz/wid,(p1.y-bott)*numTilesVert/ht);
			q2.set((p2.x-left)*numTilesHoriz/wid,(p2.y-bott)*numTilesVert/ht);

			txtrVert[twoI].set(q1.x,q1.y);
			txtrVert[twoI +1].set(q2.x,q2.y);
        }

        for(int i = 0; i < numSegments; i++)
            for(int j = 0; j < 4; j++)
            {
                int which = 4 * i + j; // which face
                int i0 = which, i1 = i0 + 4,  i3 = 4 * i + (j + 3)%4,  i2 = i3 + 4;
                norm[which] = newell4Pts(pt[i0],pt[i1],pt[i2], pt[i3]);
                norm[which].normalize();
                face[which].vert = new VertexID[4]; assert(face[which].vert != NULL);
                face[which].nVerts = 4;
                face[which].vert[0].vertIndex = i0;// same as norm index
                face[which].vert[0].normIndex = which;
                face[which].vert[1].vertIndex = i1; 
                face[which].vert[1].normIndex = which;
                face[which].vert[2].vertIndex = i2;
                face[which].vert[2].normIndex = which;
                face[which].vert[3].vertIndex = i3;
                face[which].vert[3].normIndex = which;
                if(j == 3)
                {
                    face[which].whichTxtr = ID;
                    face[which].vert[0].txtrIndex = 2 * i;
                    face[which].vert[1].txtrIndex = 2 * i + 2;
                    face[which].vert[2].txtrIndex = 2 * i + 3;
                    face[which].vert[3].txtrIndex = 2 * i + 1;
                }
                else face[which].whichTxtr = 0;
    }
} // end, makeExtrudedQuadStrip



//*******************  added Mesh methods from hlTube.cpp  **********************
// hlTube.cpp: F.S.Hill,Jr., Nov. 22, 1998, for ECE660
// Build a "tube" mesh based on curve spine(t) that forms spine of tube in space
// The parametric form for the spine is defined through three functions
// spineX(), spineY(), and spineZ(), which user defines
// example here is for a torus knot, but use your own!!

// Also try different values for numSpinePts, numSlices, radius, etc.
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<< spineX >>>>>>>>>>>>>>>>>>>
double spineX(double t)
{
	double a  = 8,b  = 3,c  = 5,p  = 1,q  = 7;
	//return (a  + b  * cos( q  * t)) * cos(p  * t); //torus knot
	return cos( 12 * t );
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<< spineY >>>>>>>>>>>>>>>>>>>
double spineY(double t)
{
	double a  = 8,b  = 3,c  = 5,p  = 1,q  = 7;
   //return (a  + b  * cos( q * t)) * sin(p  * t); //torus knot
	return sin( 12 * t );
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<< spineZ >>>>>>>>>>>>>>>>>>>
double spineZ(double t)
{
	double a  = 8,b  = 3,c  = 5,p  = 1,q  = 7;
	//return c  * sin(q  * t); // torus knot
	return 2*t;
}

//<<<<<<<<<<<<<<<<<<<<< dot3 >>>>>>>>>>>>>>>
double dot3(Vector3 a, Vector3 b)
{ 
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//<<<<<<<<<<<<<<<<<<<<<< checkOrthogVects >>>>>>>>>>>>>>>>
void checkOrthogVects(Vector3 a, Vector3 b, Vector3 c)
{ 
	// check that system is orthonormal
	double aDotb = fabs(dot3(a, b));
	double aDotc = fabs(dot3(a, c));
	double bDotc = fabs(dot3(b, c));

	if( aDotb > 0.000001 || aDotc > 0.000001 || bDotc > 0.000001)
		cout << " Bad!! vectors NOT orthogonal!\n";

	double A = fabs(dot3(a,a) - 1.0);
	double B = fabs(dot3(b,b) - 1.0);
	double C = fabs(dot3(c,c) - 1.0);
 
	/*
	if(A > 0.0000001)
		cout << "Bad!! first is not normalized!\n";
	if( B > 0.0000001)
		cout << "Bad!! second is not normalized!\n";
	if( C > 0.0000001)
		cout << "Bad!! third is not normalized!\n";
	*/ 
}

//######################### MAKE SMOOTH TUBE ######################
void Mesh:: makeSmoothTube(void)
{  // make tube: wrap polygon about spine
	//int numSpinePts = 200;
	int numSpinePts = 200;
	
	//int numSlices = 12; // # vrtices in polygon
	int numSlices = 20; // # vrtices in polygon

	//float radius = 1.0;
	float radius = 0.25;

	float fractForInnerOuterRadii = 0.4f; // use inner-outer radii n-gon for polygon

	//---------------------------- make space for lists -----------------
	numVerts = numSlices *  numSpinePts ; // total # vertices in mesh
	numFaces = numSlices * (numSpinePts -1);       // total # faces in mesh
	numNorms = numVerts;
	pt   = new Point3[numVerts];  assert(pt   != NULL); // make space for the lists
	face = new Face[numFaces];    assert(face != NULL);
	norm = new Vector3[numNorms]; assert(norm != NULL);

	//make polygon that wraps spine:
	Point2 *p = new Point2[numSlices];  assert(p);// place for polygon that wraps
	//Point3 *p = new Point3[numSlices];  assert(p);// place for polygon that wraps

	
	float ang = 0, delAng = TWOPI/numSlices;
	for(int j = 0; j < numSlices; j++, ang += delAng) // numSlices should be even
	{	
		float rad = radius;
		//if(j%2) rad = fractForInnerOuterRadii * radius; //alternating radius
		p[j].x = rad * cos(ang);
		p[j].y = rad * sin(ang);
	}

	double t = 0, tMax, delT, eps = 0.0001;
	tMax = TWOPI;
	delT = tMax/(numSpinePts-1);
	int last = -1;

	// loop over values of t for different spine segments
	for( int k = 0; k < numSpinePts; k++, t += delT ) // for each spine point..
	{
		Point3 spine(spineX(t), spineY(t), spineZ(t));
	
		//make Frenet frame at spine points:
		Vector3 tangent, spinePrimePrime, B, N;

		//do tangent with numerical derivative
		double xderiv = (spineX(t + eps) - spineX(t - eps))/(2*eps); 
		double yderiv = (spineY(t + eps) - spineY(t - eps))/(2*eps); 
		double zderiv = (spineZ(t + eps) - spineZ(t - eps))/(2*eps); 
		tangent.set(xderiv,yderiv,zderiv);tangent.normalize();

		// second differences for the acceleration
		double xDoublePrime = (spineX(t - eps) - 2 * spineX(t) + spineX(t + eps))/(eps*eps);
		double yDoublePrime = (spineY(t - eps) - 2 * spineY(t) + spineY(t + eps))/(eps*eps);
		double zDoublePrime = (spineZ(t - eps) - 2 * spineZ(t) + spineZ(t + eps))/(eps*eps);

		spinePrimePrime.set(xDoublePrime, yDoublePrime, zDoublePrime);
		B = tangent.cross(spinePrimePrime);	B.normalize();
		N = B.cross(tangent);N.normalize();

		checkOrthogVects(tangent,B,N); // are tangent, B, and n orthonormal?

		//compute vertex list
		for(int j = 0; j < numSlices; j++)
		{	
			Vector3 V(p[j].x * B.x + p[j].y * N.x, // construct new vector
				        p[j].x * B.y + p[j].y * N.y,
				        p[j].x * B.z + p[j].y * N.z);

			pt[++last].set(spine.x + V.x, spine.y + V.y, spine.z + V.z);
			norm[last].set(V.x, V.y, V.z);
			norm[last].normalize();

		} // end: for each polygon vertex at this spine point

	} // end: for each spine segment

	// ----------------- make face lists --------------------
	int ind = -1;
	for(int spinePt = 0; spinePt < numSpinePts - 1; spinePt++)
	{
		for(int f = 0; f < numSlices; f++) // for each face in this segment
		{
			face[++ind].nVerts = 4;
			face[ind].vert = new VertexID[4]; assert(face[ind].vert != NULL);
			int i0 = spinePt * numSlices + f; // first index of the vert in this face
			int i1 = (f == numSlices - 1) ? spinePt * numSlices :(i0 + 1); 
			int i2 = i1 + numSlices;
			int i3 = i0 + numSlices;  // last index of the vert in this face
			face[ind].vert[0].vertIndex = face[ind].vert[0].normIndex = i0;
			face[ind].vert[1].vertIndex = face[ind].vert[1].normIndex = i1;
			face[ind].vert[2].vertIndex = face[ind].vert[2].normIndex = i2;
			face[ind].vert[3].vertIndex = face[ind].vert[3].normIndex = i3;	
			
		} // end: for each face in this segment

	} // end: for each spine segment - building face and normal lists

} // end: makeTube()

//****************************************************

// Scene methods
//<<<<<<<< methods >>>>>>>>>>>
string Scene :: nexttoken(void) //########## nexttoken()
{
	char c;
	string token;
	int lastchar = 1;
	if (!f_in) {return(token); }
	if (f_in->eof()) {return(token);}
	while (f_in->get(c)) 
	{
		if (f_in->eof()) {
			return(token);
		}
		switch (c) {
		case '\n': nextline += 1;
		case ' ' :
		case '\t':
		case '\a':
		case '\b':
		case '\v':
		case '\f':
		case '\r': {
			if ( lastchar == 0 ) {return(token);}break; }
		case '{': {
			token = c; return(token); break;}
		case '}': {
			token = c;
			return(token);
			break; }
		case '!': {
			while ( c != '\n' && f_in->get(c)) {
			}
			nextline++; break;}
		default: {
			token = token + c;
			lastchar = 0;
			
			if ((f_in->peek() == '{') ||
				(f_in->peek() == '}') ) {
				if ( lastchar == 0 ) {
					return(token);
				} else {
					f_in->get(c);
					token = c;
					return(token);
				}
			}
			line = nextline;
					}
		}
	}
	return(" ");
}

//<<<<<<<<<<<<<< getFloat >>>>>>>>>>>>>>>>
float Scene :: getFloat() //############ getFloat()
{
	strstream tmp;
	float number;
	string str = nexttoken();  
	tmp << str;
	if(!(tmp >> number))
	{
		cerr << "Line " << line << ": error getting float" << endl;
		exit(-1);
	}
	else 
	{
		char t;
		if ( (tmp >> t ) ) 
		{
			cerr << "Line " << line << ": bum chars in number" << endl;
			exit(-1);    
		} 
	}
	return number;
}

//<<<<<<<<<<<<<<<<< isidentifier >>>>>>>>>>>>>>>>
bool Scene :: isidentifier(string keyword) { //######## isidentifier
	string temp = keyword;
	if (!isalpha(temp[0])) return(false);
	for (int count = 1; count < temp.length(); count++) {
		if ((!isalnum(temp[count]))&& (temp[count]!='.')) return(false);
	}
	return(true);
}

//<<<<<<<<<<<<<<<< cleanUp >>>>>>>>>>>>>>>>
void Scene :: cleanUp() //######### cleanUp
{ // release stuff after parsing file
	affStk.releaseAffines(); 		//delete affine stack
	def_stack->release();
	delete def_stack; // release the DefUnitStack memory
}

//<<<<<<<<<<<<<<<<< freeScene >>>>>>>>>>>>>>
void Scene :: freeScene() 
{ // release the object and light lists
	GeomObj *p = obj;
	while(p)
	{
		GeomObj* q = p;
		p = p->next;
		delete q;
	}
	Light * q = light;
	while(q)
	{
		Light* r = q;
		q = q->next;
		delete r;
	}
}

//<<<<<<<<<<<<<<<<< whichToken >>>>>>>>>>>>>>>
mTokenType Scene :: whichtoken(string keyword) 
{
	string temp = keyword;
	if ( temp == "light" )				 return LIGHT;
	if ( temp == "rotate" )           return ROTATE;
	if ( temp == "translate" )        return TRANSLATE;
	if ( temp == "scale")             return (SCALE);	
	if ( temp == "push")        		 return (PUSH);
	if ( temp == "pop")					 return (POP);
	if ( temp == "identityAffine")    return (IDENTITYAFFINE);
	if ( temp == "cube")              return (CUBE);
	if ( temp == "sphere")            return (SPHERE);
	if ( temp == "torus")             return (TORUS);
	if ( temp == "plane")             return (PLANE);
	if ( temp == "square")            return (SQUARE);
	if ( temp == "cylinder")          return (CYLINDER);
	if ( temp == "taperedCylinder")   return (TAPEREDCYLINDER);
	if ( temp == "cone")              return (CONE);
	if ( temp == "tetrahedron")       return (TETRAHEDRON);
	if ( temp == "octahedron")        return (OCTAHEDRON);
	if ( temp == "dodecahedron")      return (DODECAHEDRON);
	if ( temp == "icosahedron")       return (ICOSAHEDRON);
	if ( temp == "buckyball")         return (BUCKYBALL);
	if ( temp == "diamond")           return (DIAMOND);
	if ( temp == "teapot")				 return (TEAPOT);
	if ( temp == "union")             return (UNION);
	if ( temp == "intersection")      return (INTERSECTION);
	if ( temp == "difference")        return (DIFFERENCEa);
	if ( temp == "mesh")              return (MESH);

	if ( temp == "extrude" )			  return (EXTRUDE);
	if ( temp == "texturedCube" )	  return (TEXTUREDCUBE);

	if ( temp == "makePixmap")        return (MAKEPIXMAP);
	if ( temp == "defaultMaterials")  return (DEFAULTMATERIALS);
	if ( temp == "ambient")           return (AMBIENT);
	if ( temp == "diffuse")           return (DIFFUSE);
	if ( temp == "specular")          return (SPECULAR);
	if ( temp == "specularFraction")  return (SPECULARFRACTION);
	if ( temp == "surfaceRoughness")  return (SURFACEROUGHNESS);
	if ( temp == "emissive")          return (EMISSIVE);
	if ( temp == "specularExponent")  return (SPECULAREXPONENT);
	if ( temp == "speedOfLight")      return (SPEEDOFLIGHT);
	if ( temp == "transparency")      return (TRANSPARENCY);
	if ( temp == "reflectivity")      return (REFLECTIVITY);
	if ( temp == "parameters")        return (PARAMETERS);
	if ( temp == "texture")				 return (TEXTURE);
	if ( temp == "globalAmbient")	    return (GLOBALAMBIENT);
	if ( temp == "minReflectivity")	 return (MINREFLECTIVITY);
	if ( temp == "minTransparency")	 return (MINTRANSPARENCY);
	if ( temp == "maxRecursionDepth") return (MAXRECURSIONDEPTH);
	if ( temp == "background")        return (BACKGROUND);
	if ( temp == "{")                 return (LFTCURLY);
	if ( temp == "}")                 return (RGHTCURLY);
	if ( temp == "def")               return (DEF);
	if ( temp == "use")               return (USE);
	if ( temp == " " )                return (T_NULL);

	if( temp == "pawn" )			  return (PAWN);

	if ( isidentifier(temp) )         return (IDENT);


	cout << temp << ":" << temp.length() << endl;
	return(UNKNOWN);
} // end of whichtoken

/* alternative approach - doesn't work yet
//<<<<<<<<<<<<<<<<<<<< whichToken >>>>>>>>>>>>>>>>>
mTokenType Scene :: whichtoken(string& keyword) 
{
string sList[] = {"light","rotate","translate", "scale","push","pop",
"identityAffine","cube","sphere","torus","plane","square","cylinder",
"taperedCylinder","cone","tetrahedron","octahedron","dodecahedron","icosahedron",
"buckyball","diamond","teapot","union","intersection","difference","mesh",
"makePixmap","defaultMaterials","ambient","diffuse","specular","specularFraction",
"surfaceRoughness","emissive","specularExponent","speedOfLight","transparency",
"reflectivity","parameters","texture","globalAmbient","minReflectivity",
"minTransparency","maxRecursionDepth","background","{","}","def","use"," "};

  mTokenType tList[] = {LIGHT, ROTATE, TRANSLATE, SCALE,PUSH,
  POP,IDENTITYAFFINE,CUBE,SPHERE,TORUS,PLANE,SQUARE,CYLINDER,TAPEREDCYLINDER,CONE,
  TETRAHEDRON,OCTAHEDRON,DODECAHEDRON,ICOSAHEDRON,BUCKYBALL,DIAMOND,TEAPOT,UNION,
  INTERSECTION,DIFFERENCEa,MESH,MAKEPIXMAP,DEFAULTMATERIALS,AMBIENT,DIFFUSE,
  SPECULAR,SPECULARFRACTION,SURFACEROUGHNESS,EMISSIVE,SPECULAREXPONENT,SPEEDOFLIGHT,
  TRANSPARENCY,REFLECTIVITY,PARAMETERS,TEXTURE,GLOBALAMBIENT,MINREFLECTIVITY,
  MINTRANSPARENCY,MAXRECURSIONDEPTH,BACKGROUND,LFTCURLY,RGHTCURLY,DEF,USE,T_NULL};
  
	 //string temp = keyword;
	 cout << "\nread: " << keyword;
	 for(int i = 0; i < 42; i++)
	 {
	 cout << "\ni = " << i;
	 if(keyword == sList[i]) return tList[i];
	 }
	 if (isidentifier(keyword) ) return (IDENT);
	 cout << keyword << ":" << keyword.length() << endl;
	 return UNKNOWN;
	 }
*/

//<<<<<<<<<<  drawSceneOpenGL >>>>>>>>>>>>>>>>.
void Scene :: drawSceneOpenGL()
{ //draw each object on object list
			for(GeomObj* p = obj; p ; p = p->next)
				p->drawOpenGL(); //draw it
}

//<<<<<<<<<<<<<<< Scene :: read >>>>>>>>>>>>>>>>
bool Scene:: read(string fname)// return true if ok; else false
{
	file_in = new ifstream(fname.c_str());

	cout << "opening the file " << fname << "\n";

	if(!(*file_in))
	{ 
		cout << "I can't find or open file: " << fname << endl; 
		return false;
	}

	cout << "File opened ok\n";

	f_in = new strstream();
	line = nextline = 1;
	def_stack = new DefUnitStack();
	char ch;
	freeScene(); //delete any previous scene	
	// initialize all for reading:
	obj = tail = NULL;
	light = NULL;
	affStk.tos = new AffineNode;
	affStk.tos->next = NULL;
	
	while (file_in->get(ch)) {*f_in << ch;} // read whole file
	while(1) //read file, collecting objects, until EOF or an error
	{
		GeomObj * shp = getObject(); // get the next shape
		if(!shp) break; // no object: either error or EOF
		shp->next = NULL; // to be safe
		if(obj == NULL){ obj = tail = shp;} // empty list so far
		else{tail->next = shp; tail = shp;} // add new object to queue
	}	
	file_in->close();
	cleanUp(); // delete temp lists, etc.
	return true;
} // end of read()

//<<<<<<<<<<<<<< Scene :: getObject >>>>>>>>>>>>>>>	
GeomObj* Scene :: getObject()
{ //reads tokens from stream f_in (a data member of Scene),
	// building lights, getting materials, doing transformations,
	// until it finds a new object
	// returns NULL if any error occurs, or end of file
	string s;			
	GeomObj * newShape;
	mTokenType typ;
	while ((typ = (whichtoken( s = nexttoken() ))) != T_NULL) 
	{
		if(typ == UNION || typ == INTERSECTION || typ == DIFFERENCEa)
		{
			switch(typ)
			{
			case UNION:				newShape = new UnionBool();	break;
			case INTERSECTION:	newShape = new IntersectionBool();	break;
			case DIFFERENCEa:		newShape = new DifferenceBool();break;
			} // end of little switch
			GeomObj* p = newShape;
			p = getObject(); // get left child
			if(!p) return NULL; // Error! should always get an object
			((Boolean*)newShape)->left  = p; // hook it up
			p = getObject();// get right child
			if(!p) return NULL;
			((Boolean*)newShape)->right = p; // hook it up
			return newShape;
		}// end of if(typ == UNION etc....
		switch(typ) 
		{
		case LIGHT: {
			Point3 p;
			Color3 c;
			p.x = getFloat(); p.y = getFloat();	p.z = getFloat();
			c.red = getFloat(); c.green = getFloat();	c.blue = getFloat();
			Light *l = new Light;
			l->setPosition(p);
			l->setColor(c);
			l->next = light; //put it on the list
			light = l; break;}
		case ROTATE: {
			float angle;
			Vector3 u;
			angle = getFloat(); u.x = getFloat(); 
			u.y = getFloat(); u.z = getFloat();
			affStk.rotate(angle,u);break;}
		case TRANSLATE: {
			Vector3 d;
			d.x = getFloat(); d.y = getFloat(); d.z = getFloat();
			affStk.translate(d);break;}
		case SCALE: {
			float sx, sy, sz;
			sx = getFloat(); sy = getFloat(); sz = getFloat();
			affStk.scale(sx, sy, sz);break;}
		case PUSH: affStk.dup(); break; 
		case POP:  affStk.popAndDrop(); break;
		case IDENTITYAFFINE: affStk.setIdentity();break;		
		case AMBIENT: {
			float dr, dg, db;
			dr = getFloat(); dg = getFloat(); db = getFloat();
			currMtrl.ambient.set(dr,dg,db); break;}
		case DIFFUSE: {
			float dr,dg,db;
			dr = getFloat(); dg = getFloat(); db = getFloat();
			currMtrl.diffuse.set(dr,dg,db); break;}
		case SPECULAR:{ 
			float dr,dg,db;
			dr = getFloat(); dg = getFloat(); db = getFloat();
			currMtrl.specular.set(dr,dg,db); break;}
		case EMISSIVE: {
			float dr,dg,db;
			dr = getFloat(); dg = getFloat(); db = getFloat();
			currMtrl.emissive.set(dr,dg,db); break;}
		case PARAMETERS: { // get a list of numParams parameters
			currMtrl.numParams = (int)getFloat();
			for(int i = 0; i < currMtrl.numParams; i++)
				currMtrl.params[i] = getFloat();
			break;}			
		case SPECULARFRACTION: currMtrl.specularFraction = getFloat(); break;
		case SURFACEROUGHNESS: currMtrl.surfaceRoughness = getFloat(); break;
		case TEXTURE: { // get type, 0 for none
			currMtrl.textureType = getFloat();}	
			break;
		case DEFAULTMATERIALS: 	currMtrl.setDefault();break;
		case SPEEDOFLIGHT: currMtrl.speedOfLight = getFloat(); break;
		case SPECULAREXPONENT: currMtrl.specularExponent = getFloat(); break;
		case TRANSPARENCY:currMtrl.transparency = getFloat(); break;
		case REFLECTIVITY: currMtrl.reflectivity = getFloat(); break;
		case GLOBALAMBIENT:
			ambient.red = getFloat(); ambient.green = getFloat();
			ambient.blue = getFloat(); break;
		case BACKGROUND: 
			background.red = getFloat();
			background.green = getFloat(); 
			background.blue = getFloat();break;
		case MINREFLECTIVITY: minReflectivity = getFloat(); break;
		case MINTRANSPARENCY:minTransparency = getFloat(); break;
		case MAXRECURSIONDEPTH: maxRecursionDepth = getFloat(); break;
		
		case MAKEPIXMAP: 
			{	
				// get BMP file name for a pixmap			
				// to be implemented, along the lines:
				int which = (int) getFloat();		// index of this pixmap in pixmap array

				if(which < 0 || which > 7)
				{
					cout << "\nbad index of RGBpixmap!\n";
				}
				string fname = nexttoken();		// get file name for texture	  
				cout << "I got fname = " << fname << endl;
				if( !pixmap[which].readBMPFile(fname) )
				{
					// read BMP file into this pixmap
					cout << " \ncan't read that RGBpixmap file!\n";
					return NULL;  
				}
				int textureId = (int) getFloat();
			//	for( int i = 0; i < 6; i++ )
			//	{
					pixmap[which].setTexture( textureId );
			//		textureId = (int) getFloat();
			//	}
				cout << "End of the PIXMAP case\n.";
				break;
			}// end of case: MAKEPIXMAP
			
		case T_NULL: break; // The null token represents end-of-file 
		case DEF: {
			string name, temp, lb, rb;
			int l = line;
			string inp;
			name = nexttoken();
			if ( whichtoken(name) != IDENT ) {
				cout << "Error:  Identifier expected." << endl;
				return NULL;
			}
			if ( def_stack->search(name) ) {
				cout << line << ": " << name;
				cout << ": attempt to redefine. " << endl;
				return NULL;
			}
			lb = nexttoken();
			if ( whichtoken(lb) != LFTCURLY ) {
				cout << "Error: { expected." << endl;
				return NULL;
			}
			while ( whichtoken( temp = nexttoken()) != RGHTCURLY ) {
				cout << temp << endl;
				inp = inp + temp + " ";
				if (!f_in) {
					cout << "Error: end of file detected." << endl;
					return NULL;
				}
			}
			// Push the contents of the string on the stack.
			def_stack->push(name, inp);
			break;} // end of case: DEF
		case USE: {
			string name;
			name = nexttoken();
			if ( whichtoken(name) != IDENT ) {
				cout << line << ": " << name;
				cout << ": identifier expected.";
				return NULL;
			}
			if (! def_stack->search(name) ) {
				cout << line << ": " << name;
				cout << ": not defined.";
				return NULL;
			}
			cout << def_stack->contents(name) << endl;
			strstream *temp_fin = new strstream;
			*temp_fin << def_stack->contents(name) << " ";
			*temp_fin << f_in->rdbuf();
			delete (f_in);
			f_in = temp_fin;
			break; } // end of case: USE
		default:  { // inner switch for Shapes
			switch(typ) 
			{
				//float param;
			case CUBE:			newShape = new Cube;break;
			case SPHERE:		newShape = new Sphere;break;
			case TETRAHEDRON:	newShape = new Mesh("tetra.3vn");break;
			case TORUS:		newShape = new Torus;break;
			case PLANE:		newShape = new Plane;break;
			case SQUARE:		newShape = new Square;break;
			case TAPEREDCYLINDER:	newShape = new TaperedCylinder; 
				((TaperedCylinder*)newShape)->smallRadius = getFloat(); break;
			case CONE:			newShape = new TaperedCylinder; 
				((TaperedCylinder*)newShape)->smallRadius = 0; break;
			case CYLINDER:		newShape = new TaperedCylinder; 
				((TaperedCylinder*)newShape)->smallRadius = 1; break;
			case OCTAHEDRON:		newShape = new Mesh("octa.3vn");break;
			case DODECAHEDRON:	newShape = new Mesh("dodeca.3vn"); break;
			case ICOSAHEDRON:	newShape = new Mesh("icosa.3vn"); break;
			case BUCKYBALL:		newShape = new Mesh("bucky.3vn");	break;
			case DIAMOND:		newShape = new Mesh("diamond.3vn"); break;

			case PAWN:			newShape = new Mesh( "pawn.3vn" ); break;

			case TEAPOT:		newShape = new Teapot; break;
			case MESH: 
				{
					// get a filename (with extension) for this mesh		
					string fname = nexttoken(); // get file name for mesh
					newShape = new Mesh(fname); break;
				}// end of case: MESH
			case EXTRUDE: 
				{ 
					//get a filename with extension for this mesh
					string fname = nexttoken();
					newShape = new Extrude(fname); 
					break;
				} // end of case: MESH
			
			case TEXTUREDCUBE:
				{
					int textureId;
					int arr[6];
					for( int i = 0; i < 6; i++ )
					{
 						arr[i] = (int) getFloat();
					}
					newShape = new TexturedCube( arr );

				}
				break;
				
			default: {
				cerr << "Line " << nextline << ": unknown keyword " << s << endl;
				return NULL;
						}	
			} // end of inner switch
			// common things to do to all Shape’s
			((Shape*)newShape)->mtrl.set(currMtrl);
			// load transform and its inverse
			((Shape*)newShape)->transf.set(*(affStk.tos->affn));
			((Shape*)newShape)->invTransf.set(*(affStk.tos->invAffn));
			return newShape;
					 }// end of default: block			
    } // end of outer switch
  } // end of while
  return NULL;
} // end of getObject

//<<<<<<<<<<<<<<<<< makeLightsOpenGL >>>>>>>>>>>>>>> 
void Scene:: makeLightsOpenGL() 
{// build lights from scene's light list 
  /* in GL.h: GL_LIGHT0..GL_LIGHT7 are defined as consecutive ints from 0x4000 to 0x4007 */ 

	int num, lightNum = GL_LIGHT0; 
	Light *p; 
	float colr[4], posn[4]; // arrays to hold openGL color and position 
	for(p = light, num = 0; p && num < 8; p = p-> next, lightNum++, num++) 
	{ // set up to 8 OpenGL lights 
		glEnable(lightNum); 
		(p->color).build4tuple(colr); 
		(p->pos).build4tuple(posn); 
		glLightfv(lightNum,GL_DIFFUSE,colr); 
		glLightfv(lightNum,GL_SPECULAR,colr); 
		glLightfv(lightNum,GL_POSITION,posn);

		glClearColor( background.red, background.green, background.blue, 0.0f );	// added 8/29
	}
} // end of makeLightsOpenGL 

// DefUnitStack methods
//******************************************************************
void DefUnitStack :: push(string n, string s) { 
	D4S *temp_d4s = new D4S;
	temp_d4s->current = new DefUnit(n, s);
	temp_d4s->next = stack;
	stack = temp_d4s;
}

//******************************************************************
void DefUnitStack :: print() {
	D4S *temp = stack;
	string t;
	while (temp) {
		cout << temp->current->name << ":" ;
		cout << temp->current->stuff << endl;
		temp = temp->next;
	}
}

//******************************************************************
int DefUnitStack :: search(string s) {
	D4S *temp = stack;
	while (temp) {
		if ( temp->current->name == s ) {
			return(1);
		}
		temp = temp->next;
	}
	return(0);
}

//******************************************************************
string DefUnitStack :: contents(string s) {
	D4S *temp = stack;
	while (temp) {
		if (temp->current->name == s ) {
			return(temp->current->stuff);
		}
		temp = temp->next;
	}
	return(NULL);
}

//******************************************************************
void DefUnitStack :: release()
{
	while(stack)
	{
		D4S* tmp = stack; // grab it
		//cerr << "releasing def_stack item: "<< tmp->current->name<< endl;
		stack = stack->next; // advance p
		delete tmp->current; // release 2 strings
		delete tmp; // release node
	}
	stack = NULL;
}