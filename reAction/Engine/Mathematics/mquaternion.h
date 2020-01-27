


#ifndef MQUATERNION_H
#define MQUATERNION_H

#include "mvector.h"
// a quaternion class
// created by mixing http://www.3dkingdoms.com/weekly/quat.h
// with book appendix c "physics for game developers" david m.bourg

class quat
{
public:
	float x,y,z,w;

	quat	();
	quat	(float fx,float fy,float fz,float fw);
	quat	(float angle, const vec3 &axis);		//axis angle quat constructor

	float	magnitude();

	quat	operator+=	(quat &q);
	quat	operator-=	(quat &q);
	quat	operator*=	(float s);
	quat	operator/=	(float s);
	quat	operator~	(void) const;

	quat	operator*	(float s) const;

	quat	operator*	(const quat &b) const;
	quat	operator+	(const quat &b) const;
	quat	operator-	(const quat &b) const;
	quat	operator/	(float s) const;

	float	getangle();
	vec3	getaxis();
	int		normalize();
	void	tomatrix( float * mat );
	void	setaxis(float fX,float fY,float fZ,float deg); // set axis-angle quat
	void	reset();

	vec3	getvector();


};

quat		qrotate		(const quat &q1,const quat &q2);
quat		qvrotate	(const quat &q,const vec3 &v);
quat		qfromeuler	(float x,float y,float z);
vec3		eulerfromq	(const quat &q);
float		qdot		(const quat &q1,const quat &q2);
quat		qslerp		(const quat &q1,const quat &q2,float t); // spherical linear interpolation
quat		qlerp		(const quat &q1,const quat &q2,float t); // linear interpolation


#endif