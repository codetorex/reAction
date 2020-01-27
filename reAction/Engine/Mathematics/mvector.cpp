

#include "StdAfx.h"
#include "mvector.h"

// vec2 functions
vec2::vec2()
{
	x = 0.0f;
	y = 0.0f;
}

vec2::vec2(float x_,float y_)
{
	x = x_;
	y = y_;
}

vec2& vec2::operator = (   float *other )
{
	x = other[0];
	y = other[1];
    return *this;
}

vec2& vec2::operator = (   const vec2 & other )
{
	x = other.x;
	y = other.y;
    return *this;
}

vec2& vec2::operator = (   const vec3 & other )
{
	x = other.x;
	y = other.y;
    return *this;
}

vec2 vec2::operator + ( const vec2 &other )
{
    vec2 vResult;
	
    vResult.x = x + other.x;
    vResult.y = y + other.y;
	
    return vResult;
}

vec2 vec2::operator - (   const vec2 &other )
{
    vec2 vResult(0.0f, 0.0f);
	
    vResult.x = x - other.x;
    vResult.y = y - other.y;
	
    return vResult;
}

vec2 vec2::operator - ( void ) const
{
    vec2 vResult(-x, -y);
	
    return vResult;
}

vec2 vec2::operator * (   const vec2 &other )
{
    vec2 vResult(0.0f, 0.0f);
	
    vResult.x = x * other.x;
    vResult.y = y * other.y;
	
    return vResult;
}

vec2 vec2::operator * (  const float factor )
{
    vec2 vResult(0.0f, 0.0f);
	
    vResult.x = x * factor;
    vResult.y = y * factor;

    return vResult;
}

vec2 vec2::operator / ( const float factor )
{
    vec2 vResult(0.0f, 0.0f);
	
    vResult.x = x / factor;
    vResult.y = y / factor;
	
    return vResult;
}

float vec2::length()
{
	return( (float)sqrt( x * x + y * y ) );
}

void vec2::normalize()
{
	float fLength = length();
	
	x = x / fLength;
	y = y / fLength;
}

float dot2 (  const vec2 & v1,   const vec2 & v2 )
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

float cross2 (  const vec2 & v1,   const vec2 & v2 )
{
	return (v1.x * v2.y - v1.y * v2.x);
}


vec2::operator vec3()
{
	vec3 rt;
	rt.x = x;
	rt.y = y;
	rt.z = 0.0f;
	return (rt);
}

void vec2::set(float x_, float y_)
{
	x = x_;
	y = y_;
}

void vec2::set(float *k)
{
	x = k[0];
	y = k[1];
}


// vec3 functions
vec3::vec3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

vec3::vec3(float x_,float y_,float z_)
{
	x = x_;
	y = y_;
	z = z_;
}

void vec3::set(float x_, float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}

float vec3::length()
{
	return( (float)sqrt( x * x + y * y + z * z ) );
}

void vec3::normalize()
{
	float fLength = length();

	x = x / fLength;
	y = y / fLength;
	z = z / fLength;
}

vec3 vec3::operator + ( const vec3 &other )
{
    vec3 vResult;

    vResult.x = x + other.x;
    vResult.y = y + other.y;
    vResult.z = z + other.z;

    return vResult;
}

vec3 vec3::operator - (   const vec3 &other )
{
    vec3 vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x - other.x;
    vResult.y = y - other.y;
    vResult.z = z - other.z;

    return vResult;
}

vec3 vec3::operator - ( void ) const
{
    vec3 vResult(-x, -y, -z);

    return vResult;
}

vec3 vec3::operator * (   const vec3 &other )
{
    vec3 vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x * other.x;
    vResult.y = y * other.y;
    vResult.z = z * other.z;

    return vResult;
}

vec3 vec3::operator * (  const float factor )
{
    vec3 vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x * factor;
    vResult.y = y * factor;
    vResult.z = z * factor;

    return vResult;
}

vec3 vec3::operator / ( const float factor )
{
    vec3 vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x / factor;
    vResult.y = y / factor;
    vResult.z = z / factor;

    return vResult;
}

vec3 operator * (  const float factor,   const vec3 &other )
{
    vec3 vResult(0.0f, 0.0f, 0.0f);

    vResult.x = other.x * factor;
    vResult.y = other.y * factor;
    vResult.z = other.z * factor;

    return vResult;
}

vec3 vec3::operator / (   const vec3 & other )
{
    vec3 vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x / other.x;
    vResult.y = y / other.y;
    vResult.z = z / other.z;

    return vResult;
}

vec3& vec3::operator = (   const vec3 & other )
{
	x = other.x;
	y = other.y;
	z = other.z;

    return *this;
}

vec3& vec3::operator = (   const vec2 & other )
{
	x = other.x;
	y = other.y;
	z = 0.0f;

    return *this;
}

vec3& vec3::operator = (   float *other )
{
	x = other[0];
	y = other[1];
	z = other[2];

    return *this;
}


vec3& vec3::operator += (   const vec3 & other )
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

vec3& vec3::operator -= (   const vec3 & other )
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
}

void vec3::set(float *k)
{
	x = k[0];
	y = k[1];
	z = k[2];
}

void vec3::render()
{
	glVertex3f(x,y,z);
}

float distance(  const vec3 & v1,   const vec3 & v2  )
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	float dz = v1.z - v2.z;

	return (float)sqrt( dx * dx + dy * dy + dz * dz );
}

float dot(  const vec3 & v1,   const vec3 & v2 )
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

vec3 cross(  const vec3 & v1,   const vec3 & v2 )
{
	vec3 vCrossProduct;

	vCrossProduct.x =  v1.y * v2.z - v1.z * v2.y;
	vCrossProduct.y = -v1.x * v2.z + v1.z * v2.x;
	vCrossProduct.z =  v1.x * v2.y - v1.y * v2.x;

	return vCrossProduct;
}

vec3 mv3f(float x,float y,float z)
{
	vec3 temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	return temp;
};

vec3 eline( const vec3 & src, const vec3 & dest,float mult)
{
	vec3 resul = src;
	resul -= (mult * dest);
	return resul;
}


vec3 linip( const vec3 & vsrc, const vec3 & vdest,float tween)
{
	vec3 tw;
	tw = (tween * vdest) + ((1.0f-tween) * vsrc);
	return tw;
}

vec3	reflect	(const vec3 &I,const vec3 &N,const vec3 &RN)
{
	float n1,n2;
	vec3 New_Ray;

	n1 = dot(I,N);
	n1 *= -2.0f;
	New_Ray = (n1 * N) + I ; 


	n1 = dot(New_Ray,RN);

	if (n1 < 0.0f)
	{

		n2 = dot(New_Ray,N);
		if (n2 < 0.0f)
		{
			n1 = dot(I,RN);
			n1 *= -2.0f;
			New_Ray = (n1 * RN) + I ; 
		}
		else
		{
			n1 *= -2.0;
			New_Ray = (n1 * RN) + I ; 
		}
	}

	
	
	//res = I;
	//res -= ((2.0f * dot(N,I)) * N);

	return New_Ray;
}
/*vec3 reflect3 ( const vec3 &I, const vec3 &N)
{

}*/

vec3 reflect (const vec3& I,const vec3& N)
{
	vec3 res,r2;
	float k;
	k = 2.0f *  dot(N,I);

	r2 = k * N;
	res = I;
	res -= r2;
	return res;
}


vec3 reflect2 (const vec3& I,const vec3& N)
{
	vec3 res,r2;
	float k;
	k = dot(N,I);

	r2 = k * N;
	res = I;
	res -= r2;
	return res;
}

float angle(vec3 &v1,vec3 &v2)
{
	float k =dot(v1,v2);
	k /= v1.length() * v2.length();
	k = RADTODEG( ((float)acos(k)) );
	return k;
}

/*void ray::extend(float mult)
{
	d = eline(p,d,mult);
}

float ray::dist()
{	
	float x,y,z;
	x = p.x - d.x;
	y = p.y - d.y;
	z = p.z - d.z;

	return (float)sqrt(x*x + y*y + z*z);
}*/


#define SMALL_NUM  0.00000001 // anything that avoids division overflow

/*int intersect_RayTriangle( ray R, triangle T, vec3* I )
{
    vec3    u, v, n;             // triangle vectors
    vec3    dir, w0, w;          // ray vectors
    float     r, a, b;             // params to calc ray-plane intersect

//	sprintf(dbuffer,"bp 1");AddDbg();
    // get triangle edge vectors and plane normal
	u = T.v1 - T.v0;
	v = T.v2 - T.v0;


    n = cross(u,v);             // cross product
	if ( (n.x == 0) && (n.y == 0) && (n.z == 0) ) return -1;

	dir = R.d - R.p;
	w0 = R.p - T.v0;

    a = -dot(n,w0);
    b = dot(n,dir);
    if (fabs(b) < SMALL_NUM) {     // ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return 2;
        else return 0;             // ray disjoint from plane
    }

//	sprintf(dbuffer,"bp 2");AddDbg();
    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                   // ray goes away from triangle
        return 0;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

	vec3 tr,tz;
	tr =dir * r;
//	sprintf(dbuffer,"bp 3");AddDbg();
	tz = R.p + tr;
    I->x = tz.x; 
	I->y = tz.y; 
	I->z = tz.z; 
//	sprintf(dbuffer,"bp 4");AddDbg();
    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = dot(u,u);
    uv = dot(u,v);
    vv = dot(v,v);
	w.x = I->x - T.v0.x;
	w.y = I->y - T.v0.y;
	w.z = I->z - T.v0.z;
    wu = dot(w,u);
    wv = dot(w,v);
    D = uv * uv - uu * vv;
//	sprintf(dbuffer,"bp 5");AddDbg();
    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)        // I is outside T
        return 0;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return 0;

    return 1;                      // I is in T
}*/

void cperp (vec3 *v1)
{
	if ( (v1->y >= 0.99f || v1->y <= -0.99f) && v1->x <= 0.001 && v1->z <= 0.001 )
	{
		v1->x = 0.0001f;
	}
}