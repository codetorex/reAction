
#include "StdAfx.h"
#include "mvector.h"
#include "mquaternion.h"

quat::quat()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1.0f;
}

quat::quat(float fx,float fy,float fz,float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

quat::quat(float angle, const vec3 &axis)
{
	setaxis(axis.x,axis.y,axis.z,angle);
}

int quat::normalize()
{
	float lengthSq = x * x + y * y + z * z + w * w;

	if (lengthSq == 0.0 ) return -1;
	if (lengthSq != 1.0 )
	{
		float scale = ( 1.0f / sqrtf( lengthSq ) );
		x *= scale;
		y *= scale;
		z *= scale;
		w *= scale;
		return 1;
	}
	return 0;
}


float quat::magnitude()
{
	return (float) sqrt(w*w + x*x + y*y + z*z);
}

quat quat::operator += (quat &q)
{
	w += q.w;
	x += q.x;
	y += q.y;
	z += q.z;
	return *this;
}

quat quat::operator -= (quat &q)
{
	w -= q.w;
	x -= q.x;
	y -= q.y;
	z -= q.z;
	return *this;
}

quat quat::operator *= (float s)
{
	w *= s;
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

quat quat::operator /= (float s)
{
	w /= s;
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

quat quat::operator~ (void) const
{
	quat vresult;
	vresult.x = -x;
	vresult.y = -y;
	vresult.z = -z;
	vresult.w = w;
	return vresult;
}

quat quat::operator+ (const quat &b) const
{
	quat vresult;
	vresult.x = b.x + x;
	vresult.y = b.y + y;
	vresult.z = b.z + z;
	vresult.w = b.w + w;

	return vresult;
}

quat quat::operator- (const quat &b) const
{
	quat vresult;
	vresult.x = x - b.x;
	vresult.y = y - b.y;
	vresult.z = z - b.z;
	vresult.w = w - b.w;

	return vresult;
}

quat quat::operator * (float s) const
{
	quat vresult;
	vresult.x = x * s;
	vresult.y = y * s;
	vresult.z = z * s;
	vresult.w = w * s;
	return vresult;
}

quat quat::operator * (const quat &b) const
{
	quat vresult;
	vresult.w = w*b.w - x*b.x - y*b.y - z*b.z;
	vresult.x = w*b.x + x*b.w + y*b.z - z*b.y;
	vresult.y = w*b.y + y*b.w + z*b.x - x*b.z;
	vresult.z = w*b.z + z*b.w + x*b.y - y*b.x;

	return vresult;
}

quat quat::operator / (float s) const
{
	quat vresult;
	vresult.x = x / s;
	vresult.y = y / s;
	vresult.z = z / s;
	vresult.w = w / s;
	return vresult;
}

float quat::getangle()
{
	return (float) (2*acos(w));
}

vec3 quat::getaxis()
{
	vec3 v;
	float m;

	v = getvector();
	m = v.length();

	if ( m <= 0.00001f)
	{
		return vec3();
	}
	else
	{
		return v/m;
	}
}

vec3 quat::getvector()
{
	vec3 vresult;
	vresult.x = x;
	vresult.y = y;
	vresult.z = z;
	return vresult;
}

void quat::setaxis(float fX,float fY,float fZ,float deg)
{
	float halfangle = DEGTORAD(deg) * 0.5f;
	float sinA = (float)sin (halfangle);
	w = (float) cos (halfangle);
	x = fX * sinA;
	y = fY * sinA;
	z = fZ * sinA;
}	

void quat::reset()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1.0f;
}

void quat::tomatrix( float * mat )
{
	float xx,xy,xz,xw,yy,yz,yw,zz,zw;
	xx      = x * x;
	xy      = x * y;
	xz      = x * z;
	xw      = x * w;

	yy      = y * y;
	yz      = y * z;
	yw      = y * w;

	zz      = z * z;
	zw      = z * w;

	mat[0]  = 1 - 2 * ( yy + zz );
	mat[1]  =     2 * ( xy - zw );
	mat[2]  =     2 * ( xz + yw );

	mat[4]  =     2 * ( xy + zw );
	mat[5]  = 1 - 2 * ( xx + zz );
	mat[6]  =     2 * ( yz - xw );

	mat[8]  =     2 * ( xz - yw );
	mat[9]  =     2 * ( yz + xw );
	mat[10] = 1 - 2 * ( xx + yy );

	mat[3]  = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0;
	mat[15] = 1;	
}

quat qrotate(const quat &q1,const quat &q2)
{
	return q1 * q2 * (~q1);
}

quat qvrotate(const quat &q,const vec3 &v)
{
	quat t;
	//t = q*v*(~q); todo: maybe it will needed someday fix this

	return t;
	//return (t.getvector());
}

quat qfromeuler(float x,float y,float z)
{
	quat q;
	double	roll = DEGTORAD(x);
	double	pitch = DEGTORAD(y);
	double	yaw	= DEGTORAD(z);

	double cyaw,cpitch,croll,syaw,spitch,sroll;
	double cyawcpitch,syawspitch,cyawspitch,syawcpitch;

	cyaw = cos(0.5f * yaw);
	cpitch = cos(0.5f * pitch);
	croll = cos(0.5f * roll);
	syaw = sin(0.5f * yaw);
	spitch = sin(0.5f * pitch);
	sroll = sin(0.5f * roll);

	cyawcpitch = cyaw * cpitch;
	syawspitch = syaw * spitch;
	cyawspitch = cyaw * spitch;
	syawcpitch = syaw * cpitch;

	q.w = (float) (cyawcpitch * croll + syawspitch * sroll);
	q.x = (float) (cyawcpitch * sroll - syawspitch * croll);
	q.y = (float) (cyawspitch * croll + syawcpitch * sroll);
	q.z = (float) (syawcpitch * croll - cyawspitch * sroll);

	return q;

	// alternative
	/*quat qx(-rx, CVec3( 1, 0, 0 ) );
	quat qy(-ry, CVec3( 0, 1, 0 ) );
	quat qz(-rz, CVec3( 0, 0, 1 ) );
	qz = qy * qz;
	*this = qx * qz;*/
}

vec3 eulerfromq(const quat &q)
{
	double		r11,r21,r31,r32,r33,r12,r13;
	double		q00,q11,q22,q33;
	double		tmp;
	vec3		u;

	q00 = q.w * q.w;
	q11 = q.x * q.x;
	q22 = q.y * q.y;
	q33 = q.z * q.z;

	r11 = q00 + q11 - q22 - q33;
	r21 = 2 * (q.x*q.y + q.w*q.z);
	r31 = 2 * (q.x*q.z - q.w*q.y);
	r32 = 2 * (q.y*q.z + q.w*q.x);
	r33 = q00 - q11 - q22 + q33;

	tmp = fabs(r31);
	if (tmp > 0.999999f)
	{
		r12 = 2 * (q.x*q.y - q.w*q.z);
		r13 = 2 * (q.x*q.z + q.w*q.y);

		u.x = RADTODEG(0.0f); // roll
		u.y = RADTODEG((float) (-(HALFPI) * r31/tmp)); // pitch
		u.z = RADTODEG((float) atan2(-r12,-r31*r13)); // yaw
		return u;
	}

	u.x = RADTODEG( (float) atan2(r32,r33)); // roll
	u.y = RADTODEG( (float) asin(-r31)); // piÇ
	u.z = RADTODEG( (float) atan2(r21,r11)); // yaw
	return u;
}

float qdot (const quat &q1,const quat &q2)
{
		return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

quat qslerp (const quat &q1,const quat &q2,float t)
{
/*
void Slerp(const CQuat& a, const CQuat& b, float t)
	{
  float w1, w2;

	  float cosTheta = a.Dot(b);
	  float theta    = (float)acos(cosTheta);
	  float sinTheta = (float)sin(theta);

	  if( sinTheta > 0.001f )
	  {
		w1 = float( sin( (1.0f-t)*theta ) / sinTheta);
		w2 = float( sin( t*theta) / sinTheta);
	  } else {
		// CQuat a ~= CQuat b
		w1 = 1.0f - t;
		w2 = t;
	  }

	  *this = a*w1 + b*w2;*/

	quat q;
	float w1,w2;

	float cosTheta = qdot(q1,q2);
	float theta = (float)acos(cosTheta);
	float sinTheta = (float)sin(theta);

	if (sinTheta > 0.001f)
	{
		w1 = float( sin( (1.0f-t)*theta ) / sinTheta);
		w2 = float( sin( t*theta) / sinTheta);
	}
	else
	{
		w1 = 1.0f - t;
		w2 = t;
	}

	q = q1 * w1;
	q += q2 * w2;

	//q = (q1*w1) + (q2*w2);
	return q;
}

quat qlerp(const quat &q1,const quat &q2,float t)
{
	quat q;
	float w1 = 1.0f - t;

	q = q1*w1 + q2*t;
	q.normalize();

	return q;
}