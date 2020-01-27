
#include "StdAfx.h"
#include "vcamera.h"
#include "vengine.h"

		int ScreenWidth = 1024;
		int ScreenHeight = 768;


FILE *dosya;

VCamera::VCamera()
{
   // Initialize variables...
   xPos = yPos = zPos = 0;
   xView = yView = zView = 0;
   xUp = yUp = zUp = 0;
   xStrafe = yStrafe = zStrafe = 0;
   ltim = 0;
   etim = 1.0f;
}



void VCamera::render()
{
   gluLookAt(xPos, yPos, zPos,xView, yView, zView,xUp, yUp, zUp);
//gluLookAt(xPos, yPos, zPos,xView+xPos, yView+yPos, zView+zPos,xUp, yUp, zUp);
}
void VCamera::rendertps()
{
   gluLookAt(xView,yView,zView ,xPos,yPos ,zPos ,xUp, yUp, zUp);
//gluLookAt(xPos, yPos, zPos,xView+xPos, yView+yPos, zView+zPos,xUp, yUp, zUp);
}


void VCamera::GetInput()
{
	//etim = 0.1f + (GetTickCount() - ltim)*0.01f;
	etim = (float)(GetTickCount() - ltim) / 100.0f;
	ltim = GetTickCount();

   int w = ScreenWidth >> 1;
   int h = ScreenHeight >> 1;

   if(mEngine.KeyState['W'])
      MoveCamera(UP*etim);                   

   if(mEngine.KeyState['S'])
      MoveCamera(DOWN*etim);                   

   if(mEngine.KeyState['A'])
      StrafeCam(STRAFE_LEFT*etim);              

   if(mEngine.KeyState['D'])
      StrafeCam(STRAFE_RIGHT*etim);  
   /*if(k->MouseState[0])
   {
		yPos+=UP * etim;
		yView+=UP *etim;
   }*/

   if(mEngine.MouseState[1])
   {
		yPos+=DOWN*etim;
		yView+=DOWN*etim;
   }
	

   if(mEngine.KeyState[VK_PRIOR])					// :P Up camera yap
   {
		yPos+=UP * etim;
		yView+=UP *etim;
   };

   if(mEngine.KeyState[VK_NEXT])					// :P down plz
   {
		yPos+=DOWN*etim;
		yView+=DOWN*etim;
   };

 //  if(IsKeyPress(VK_INSERT))				// :p Give Me THE CORDÝNATES PLZ
 //  {
//		dosya = fopen("cords.dat","w+t");
//		fprintf(dosya,"CORDS: %f\t%f\t%f",xPos,yPos,zPos);
//   };



   POINT mousePos;
	GetCursorPos(&mousePos);

   RotateByMouse(mousePos.x, mousePos.y, w, h);

   SetCursorPos(w, h);
}

void VCamera::SetCamera(float x, float y, float z,
                        float xv, float yv, float zv,
                        float xu, float yu, float zu)
{
   xPos = x;   yPos = y;   zPos = z;
   xView = xv; yView = yv; zView = zv;
   xUp = xu;   yUp = yu;   zUp = zu;
}

void VCamera::SetCamera(vec3& pos,vec3 &lk, vec3& up)
{
	xPos = pos.x;	yPos = pos.y;	zPos = pos.z;
	xView = lk.x;	yView = lk.y;	zView = lk.z;
	xUp = up.x;		yUp = up.y;		zUp = up.z;
}

void VCamera::MoveCamera(float direction)
{
   float xLookDirection = 0, yLookDirection = 0, zLookDirection = 0;

   xLookDirection = xView - xPos;
	yLookDirection = yView - yPos;
	zLookDirection = zView - zPos;

   float dp = 1 /(float)sqrt(xLookDirection * xLookDirection + yLookDirection * yLookDirection +
                             zLookDirection * zLookDirection);
   xLookDirection *= dp;
   yLookDirection *= dp;
   zLookDirection *= dp;

   UpdateCamera(xLookDirection, yLookDirection, zLookDirection, direction);

}


void VCamera::UpdateCamera(float xDir, float yDir, float zDir, float dir)
{
	xPos += xDir * dir;
	zPos += zDir * dir;

	xView += xDir * dir;
	zView += zDir * dir;
}


void VCamera::StrafeCam(float direction)
{
   CalculateStrafe();                      
   UpdateCamera(xStrafe, yStrafe, zStrafe, direction);  
}


void VCamera::CalculateStrafe()
{
   float xDir = 0, yDir = 0, zDir = 0;
   float xCross = 0, yCross = 0, zCross = 0;

   xDir = xView - xPos;
	yDir = yView - yPos;
	zDir = zView - zPos;

   float dp = 1 /(float)sqrt(xDir * xDir + yDir * yDir + zDir * zDir);
   xDir *= dp;
   yDir *= dp;
   zDir *= dp;

   xCross = (yDir * zUp) - (zDir * yUp);
   yCross = (zDir * xUp) - (xDir * zUp);
   zCross = (xDir * yUp) - (yDir * xUp);

   xStrafe = xCross;
   yStrafe = yCross;
   zStrafe = zCross;
}


void VCamera::RotateCamera(float AngleDir, float xSpeed, float ySpeed, float zSpeed)
{
	float xNewLookDirection = 0, yNewLookDirection = 0, zNewLookDirection = 0;
	xLookDirection = 0; yLookDirection = 0; zLookDirection = 0;
   float CosineAngle = 0, SineAngle = 0;

	GET_COS(CosineAngle, AngleDir);
   GET_SINE(SineAngle, AngleDir);

   xLookDirection = xView - xPos;
	yLookDirection = yView - yPos;
	zLookDirection = zView - zPos;

   float dp = 1 /(float)sqrt(xLookDirection * xLookDirection + yLookDirection * yLookDirection +
                             zLookDirection * zLookDirection);
   xLookDirection *= dp;
   yLookDirection *= dp;
   zLookDirection *= dp;

	xNewLookDirection = (CosineAngle + (1 - CosineAngle) * xSpeed) * xLookDirection;
	xNewLookDirection += ((1 - CosineAngle) * xSpeed * ySpeed - zSpeed * SineAngle)* yLookDirection;
	xNewLookDirection += ((1 - CosineAngle) * xSpeed * zSpeed + ySpeed * SineAngle) * zLookDirection;

	yNewLookDirection = ((1 - CosineAngle) * xSpeed * ySpeed + zSpeed * SineAngle) * xLookDirection;
	yNewLookDirection += (CosineAngle + (1 - CosineAngle) * ySpeed) * yLookDirection;
	yNewLookDirection += ((1 - CosineAngle) * ySpeed * zSpeed - xSpeed * SineAngle) * zLookDirection;

	zNewLookDirection = ((1 - CosineAngle) * xSpeed * zSpeed - ySpeed * SineAngle) * xLookDirection;
	zNewLookDirection += ((1 - CosineAngle) * ySpeed * zSpeed + xSpeed * SineAngle) * yLookDirection;
	zNewLookDirection += (CosineAngle + (1 - CosineAngle) * zSpeed) * zLookDirection;
	//myangle = (zView - zPos) / 4*(xView-xPos);
	xView = xPos + xNewLookDirection;
	yView = yPos + yNewLookDirection;
	zView = zPos + zNewLookDirection;
}


void VCamera::RotateByMouse(int mousePosX, int mousePosY, int midX, int midY)
{
	float yDirection = 0.0f;        
	float yRotation = 0.0f;   

	if((mousePosX == midX) && (mousePosY == midY))
      return;

	yDirection = (float)((midX - mousePosX)) / 1000.0f;		
	yRotation = (float)((midY - mousePosY)) / 1000.0f;		

	currentRotationAngle -= yRotation;  

	if(currentRotationAngle > 1.0f)
      {
		   currentRotationAngle = 1.0f;
         return;
      }

	if(currentRotationAngle < -1.0f)
		{
         currentRotationAngle = -1.0f;
         return;
      }

   float xDir = 0, yDir = 0, zDir = 0;
   xAxis = 0;yAxis = 0;zAxis = 0;

   xDir = xView - xPos;
	yDir = yView - yPos;
	zDir = zView - zPos;

   xAxis = (yDir * zUp) - (zDir * yUp);
   yAxis = (zDir * xUp) - (xDir * zUp);
   zAxis = (xDir * yUp) - (yDir * xUp);

   float len = 1 /(float)sqrt(xAxis * xAxis + yAxis * yAxis + zAxis * zAxis);
   xAxis *= len;
   yAxis *= len;
   zAxis *= len;

   RotateCamera(yRotation, xAxis, yAxis, zAxis);
   RotateCamera(yDirection, 0, 1, 0);
}


