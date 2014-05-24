// Camera.h for my Camera class, Nov. 19,1999, for ECE660 

#ifndef _HlCamera 
#define _HlCamera 
#include "SDL_arch.h" // needed for Vector3 definitions 

//##################### class CAMERA ###################### 
class Camera{ 
    private: 
    Point3 eye; 
    Vector3 u, v, n; 
    float aspect, nearDist, farDist, viewAngle; 
    void setModelViewMatrix(); 
    public: 
    Camera(void); 
    void roll(float angle); 
    void pitch(float angle); 
    void yaw(float angle); 
    void slide(double du, double dv, double dn); 
    void set(Point3 Eye, Point3 look, Vector3 up);
	
	void set( float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ );

    void setShape(float viewAngle, float aspect, float Near, float Far); 
    void setAspect(float asp); 
    void getShape(float& viewAngle, float& aspect, float& Near, float& Far); 
    void rotAxes(Vector3& a, Vector3& b, float angle); 
    void setDefaultCamera(); 
}; 
#endif 

