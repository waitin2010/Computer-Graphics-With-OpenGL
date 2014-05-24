#include <math.h>
#pragma  once
#define real float

//############################### class Point2 #######################
class Point2{
public:float x,y;
	   Point2(float xx, float yy){x = xx; y = yy;}
	   Point2(){ x = y = 0;}
	   void set(float xx, float yy){ x = xx; y = yy;}
};
//@@@@@@@@@@@@@@@@@@ Vector3 class @@@@@@@@@@@@@@@@
class vector3
{
public:
	vector3() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {};
	vector3( real a_X, real a_Y, real a_Z ) : x( a_X ), y( a_Y ), z( a_Z ) {};
	void set( real a_X, real a_Y, real a_Z ) { x = a_X; y = a_Y; z = a_Z; }
	void normalize() { real l = 1.0f / Length(); x *= l; y *= l; z *= l; }
	real Length() { return (real)sqrt( x * x + y * y + z * z ); }
	real SqrLength() { return x * x + y * y + z * z; }
	real dot( vector3 a_V ) { return x * a_V.x + y * a_V.y + z * a_V.z; }
	vector3 cross( vector3 b ) { return vector3( y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x ); }
	void operator += ( vector3& a_V ) { x += a_V.x; y += a_V.y; z += a_V.z; }
	void operator += ( vector3* a_V ) { x += a_V->x; y += a_V->y; z += a_V->z; }
	void operator -= ( vector3& a_V ) { x -= a_V.x; y -= a_V.y; z -= a_V.z; }
	void operator -= ( vector3* a_V ) { x -= a_V->x; y -= a_V->y; z -= a_V->z; }
	void operator *= ( real f ) { x *= f; y *= f; z *= f; }
	void operator *= ( vector3& a_V ) { x *= a_V.x; y *= a_V.y; z *= a_V.z; }
	void operator *= ( vector3* a_V ) { x *= a_V->x; y *= a_V->y; z *= a_V->z; }
	vector3 operator- () const { return vector3( -x, -y, -z ); }
	friend vector3 operator + ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ); }
	friend vector3 operator - ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ); }
	friend vector3 operator + ( const vector3& v1, vector3* v2 ) { return vector3( v1.x + v2->x, v1.y + v2->y, v1.z + v2->z ); }
	friend vector3 operator - ( const vector3& v1, vector3* v2 ) { return vector3( v1.x - v2->x, v1.y - v2->y, v1.z - v2->z ); }
	friend vector3 operator * ( const vector3& v, real f ) { return vector3( v.x * f, v.y * f, v.z * f ); }
	friend vector3 operator * ( const vector3& v1, vector3& v2 ) { return vector3( v1.x * v2.x, v1.y * v2.y, v1.z * v2.z ); }
	friend vector3 operator * ( real f, const vector3& v ) { return vector3( v.x * f, v.y * f, v.z * f ); }
	union
	{
		struct { real x, y, z; };
		struct { real r, g, b; };
		struct { real cell[3]; };
	};
};
//@@@@@@@@@@@@@@@@@@ Point3 class @@@@@@@@@@@@@@@@
class Point3{ 
public: 
	union
	{
		struct { real x, y, z; };
		struct { real r, g, b; };
		struct { real cell[3]; };
	};
	void set(float dx, float dy, float dz){x = dx; y = dy; z = dz;}
	void set(Point3& p){x = p.x; y = p.y; z = p.z;} 	
	Point3(float xx,     float yy, float zz){x = xx; y = yy; z = zz;}
	friend vector3 operator - ( const Point3& v1, const Point3& v2 ) { return vector3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ); }
	void operator =(Point3 p){x = p.x; y = p.y;z = p.z;}
	Point3(){x = y = z = 0;}
	void build4tuple(float v[])
	{// load 4-tuple with this color: v[3] = 1 for homogeneous
		v[0] = x; v[1] = y; v[2] = z; v[3] = 1.0f;
	}	
};


class plane
{
public:
	plane() : N( 0, 0, 0 ), D( 0 ) {};
	plane( vector3 a_Normal, real a_D ) : N( a_Normal ), D( a_D ) {};
	union
	{
		struct
		{
			vector3 N;
			real D;
		};
		real cell[4];
	};
};

class aabb
{
public:
	aabb() : m_Pos( vector3( 0, 0, 0 ) ), m_Size( vector3( 0, 0, 0 ) ) {};
	aabb( vector3& a_Pos, vector3& a_Size ) : m_Pos( a_Pos ), m_Size( a_Size ) {};
	void operator = (aabb &box)
	{
		m_Pos = box.GetPos();
		m_Size = box.GetSize();
	}
	vector3& GetPos() { return m_Pos; }
	vector3& GetSize() { return m_Size; }
	bool Intersect( aabb& b2 )
	{
		vector3 v1 = b2.GetPos(), v2 = b2.GetPos() + b2.GetSize();
		vector3 v3 = m_Pos, v4 = m_Pos + m_Size;
		return ((v4.x >= v1.x) && (v3.x <= v2.x) && // x-axis overlap
			(v4.y >= v1.y) && (v3.y <= v2.y) && // y-axis overlap
			(v4.z >= v1.z) && (v3.z <= v2.z));   // z-axis overlap
	}
	bool Contains( vector3 a_Pos )
	{
		vector3 v1 = m_Pos, v2 = m_Pos + m_Size;
		return ((a_Pos.x >= v1.x) && (a_Pos.x <= v2.x) &&
			(a_Pos.y >= v1.y) && (a_Pos.y <= v2.y) &&
			(a_Pos.z >= v1.z) && (a_Pos.z <= v2.z));
	}
	real w() { return m_Size.x; }
	real h() { return m_Size.y; }
	real d() { return m_Size.z; }
	real x() { return m_Pos.x; }
	real y() { return m_Pos.y; }
	real z() { return m_Pos.z; }
private:
	vector3 m_Pos, m_Size;
};
