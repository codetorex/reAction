#ifndef MTRIANGLE_H
#define MTRIANGLE_H

#include "mathlib.h"


float TriangleArea3(vec3 &v1,vec3 &v2,vec3 &v3);
float TriangleArea2(vec2 &v1,vec2 &v2,vec2 &v3);

class tri2;

// 3d triangle
class tri3
{
public:
	vec3 v1,v2,v3;

	vec3 GetVertex(int indice);

	int IntersectRay( ray3 &r, vec3 &I ); // intersects with ray and returns I
	vec3 CalculateNormal();
	vec3 CalculateCenter();

	vec2 ProjectTri2(tri2 &tri,vec3 &p);
	vec3 ProjectTri3(tri3 &tri,vec3 &p);

	float GetArea();
};


// 2d triangle
class tri2
{
public:
	vec2 v1,v2,v3;

	vec3 ProjectTri3(tri3 &tri,vec2 &p);
	vec2 ProjectTri2(tri2 &tri,vec2 &p);

	float GetArea();
};


#endif