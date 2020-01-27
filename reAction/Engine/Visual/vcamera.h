

#ifndef VCAMERA_H
#define VCAMERA_H
#include "../Mathematics/mathlib.h"          

/*define UP              0.1f          
#define DOWN            -0.1f    
#define LEFT            0.3f          
#define RIGHT           -0.3f         
#define STRAFE_LEFT     -0.1f       
#define STRAFE_RIGHT    0.1f    */  

#define UP              10       
#define DOWN            -10   
#define LEFT            30        
#define RIGHT           -30        
#define STRAFE_LEFT     -10      
#define STRAFE_RIGHT    10

#define GET_COS(a, b)   a = (float)cos(b)
#define GET_SINE(a, b)  a = (float)sin(b)


class VCamera
{
private:
	int ltim;
	float etim;

public:
	VCamera();                                   
	void render();
	void rendertps();

	void GetInput();

	void SetCamera(vec3& pos,vec3 &lk, vec3& up);
	void SetCamera(float x, float y, float z, float xv, float yv, float zv, float xu, float yu, float zu);
	void MoveCamera(float direction);               
	void UpdateCamera(float xDir, float yDir,float zDir, float dir);     
	void StrafeCam(float direction);
	void CalculateStrafe();                         
	void RotateCamera(float AngleDir, float xSpeed, float ySpeed, float zSpeed);
	void RotateByMouse(int mousePosX, int mousePosY,int midX, int midY);         

	float xPos, yPos, zPos;                      
	float xView, yView, zView;                     
	float xUp, yUp, zUp;                    
	float xStrafe, yStrafe, zStrafe;                
	float currentRotationAngle;
	float xAxis , yAxis , zAxis ;
	float xLookDirection, yLookDirection, zLookDirection;
};




#endif