

#ifndef MVECTOR_H
#define MVECTOR_H

class vec3;

class vec2
{
public:
	float x,y;

	vec2();
	vec2(float x_, float y_);

	vec2&	operator =  (float* other);
	vec2&	operator =  (const vec2 &other);
	vec2&	operator =  (const vec3 &other);

	void	set(float x_, float y_);
	void	set(float* k);

	
	vec2	operator +  (const vec2 &other);
	vec2	operator -  (const vec2 &other);
	vec2	operator *  (const vec2 &other);
	vec2	operator /  (const vec2 &other);

	vec2	operator /	(const float factor);
	vec2	operator *  (const float factor);

	vec2	operator - (void) const;

	operator	vec3	(void);

	float		length();
	void		normalize();
};

class vec3
{
public:
	float		x,y,z;

	vec3		();
	vec3		(float x_,float y_,float z_);

	void		set(float x_, float y_, float z_);
	void		set(float* k);
	float		length();
	void		normalize();

	void		render();

	vec3	operator +  (const vec3 &other);
	vec3	operator -  (const vec3 &other);
	vec3	operator *  (const vec3 &other);
	vec3	operator /  (const vec3 &other);

	
	vec3	operator /	(const float factor);
	vec3	operator *  (const float factor);
	friend vec3 operator * (const float factor, const vec3 &other);
	
	vec3&	operator =  (const vec3 &other);
	vec3&	operator =  (const vec2 &other);
	vec3&	operator =  (float* other);
	vec3&	operator += (const vec3 &other);
	vec3&	operator -= (const vec3 &other);

	vec3	operator + (void) const;
	vec3	operator - (void) const;
};

float	cross2		(const vec2 &v1, const vec2 &v2);
float	dot2		(const vec2 &v1, const vec2 &v2);

float	distance	(const vec3 &v1,const vec3 &v2);
float	dot			(const vec3 &v1,const vec3 &v2);
vec3	cross		(const vec3 &v1,const vec3 &v2);
vec3	linip		(const vec3 &vsrc,const vec3 &vdest,float tween); // linear interpolation
vec3	mv3f		(float x_,float y_,float z_);
vec3	eline		(const vec3 &vsrc,const vec3 &vdest,float factor);
vec3	reflect		(const vec3 &I,const vec3 &N);
float	angle		(vec3 &v1,vec3 &v2);
void	cperp		(vec3 *v1); // correct perpendicularness


//int intersect_RayTriangle( ray r, triangle T, vec3* I );

#endif