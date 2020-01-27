
#include "StdAfx.h"
#include "mtriangle.h"


float TriangleArea2(vec2 &v1,vec2 &v2,vec2 &v3)
{
	// todo: optimize this
	return TriangleArea3((vec3)v1,(vec3)v2,(vec3)v3);
}

float TriangleArea3(vec3 &v1,vec3 &v2,vec3 &v3)
{
	vec3 n,p;

	vec3 side0,side1;
	side0 =v1-v2;
	side1 =v2-v3;

	n = cross(side0,side1);
	p = n;
	n.normalize();

	return dot(n,p);
}

/*
 * 3D Triangle Functions Here
 *
 */
#define SMALL_NUM  0.00000001 // anything that avoids division overflow

int tri3::IntersectRay( ray3 &ray, vec3 &I )
{
    vec3    u, v, n;             // triangle vectors
    vec3    w0, w;          // ray vectors
    float     r, a, b;             // params to calc ray-plane intersect
	
    // get triangle edge vectors and plane normal
	u = v2 - v1;
	v = v3 - v1;
	
	
    n = cross(u,v);             // cross product
	if ( (n.x == 0) && (n.y == 0) && (n.z == 0) ) return -1;
	
	w0 = ray.p - v1;
	
    a = -dot(n,w0);
    b = dot(n,ray.d);
    if (fabs(b) < SMALL_NUM) {     // ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return 2;
        else return 0;             // ray disjoint from plane
    }
	
    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                   // ray goes away from triangle
        return 0;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect
	
	vec3 tr,tz;
	tr =ray.d * r;
	tz = ray.p + tr;
    I.x = tz.x; 
	I.y = tz.y; 
	I.z = tz.z; 
    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = dot(u,u);
    uv = dot(u,v);
    vv = dot(v,v);
	w.x = I.x - v1.x;
	w.y = I.y - v1.y;
	w.z = I.z - v1.z;
    wu = dot(w,u);
    wv = dot(w,v);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)        // I is outside T
        return 0;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return 0;
	
    return 1;                      // I is in T
}

vec3 tri3::CalculateNormal()
{
	vec3 n;
	n = cross(v1-v2,v2-v3);
	n.normalize();

	return n;
}

vec3 tri3::CalculateCenter()
{
	vec3 c = (v1+v2+v3) / 3.0f;
	return c;
}

vec3 tri3::GetVertex(int indice)
{
	switch (indice)
	{
		case 0:
			return v1;
		case 1:
			return v2;
		case 2:
			return v3;
		default:
			vec3 r;
			return r;
	}
}

float tri3::GetArea()
{
	return TriangleArea3(v1,v2,v3);
}

/*
 * 2D Triangle Functions Here
 *
 */

float tri2::GetArea()
{
	return TriangleArea2(v1,v2,v3);
}

vec3 tri2::ProjectTri3(tri3 &tri,vec2 &p)
{
	vec3 N,A,B,C,P;
	A = v1;
	B = v2;
	C = v3;
	P = p;

	N = cross(B-A,C-A);
	N.normalize();

	float AreaABC = dot(N,cross(B-A,C-A));
	
	// compute a
	float AreaPBC = dot(N,cross(B-P,C-P));
	float a = AreaPBC / AreaABC;
	
	// compute b
	float AreaPCA = dot(N,cross(C-P,A-P));
	float b = AreaPCA / AreaABC;
	
	// compute c
	float c = 1.0f - a - b;

	vec3 ret;
	ret.x = tri.v1.x * a + tri.v2.x * b + tri.v3.x * c;
	ret.y = tri.v1.y * a + tri.v2.y * b + tri.v3.y * c;
	ret.z = tri.v1.z * a + tri.v2.z * b + tri.v3.z * c;

	/*float AreaABC = GetArea();
	float AreaPBC = TriangleArea2(v2,v3,p);
	float AreaPCA = TriangleArea2(v1,p,v3);

	AreaABC = AreaABC;
	AreaPBC = AreaPBC;
	AreaPCA = AreaPCA;

	float a = AreaPBC / AreaABC;
	float b = AreaPCA / AreaABC;
	float c = 1.0f - a - b;

	vec3 ret;
	ret.x = tri.v1.x * a + tri.v2.x * b + tri.v3.x * c;
	ret.y = tri.v1.y * a + tri.v2.y * b + tri.v3.y * c;
	ret.z = tri.v1.z * a + tri.v2.z * b + tri.v3.z * c;*/

	return ret;
}