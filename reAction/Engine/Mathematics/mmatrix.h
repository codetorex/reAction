

#ifndef MMATRIX_H
#define MMATRIX_H

#include "mvector.h"
/*

   0   1   2   3
0 [0] [0] [0] [0] 3
1 [0] [0] [0] [0] 7
2 [0] [0] [0] [0] 11
3 [0] [0] [0] [0] 15

*/

class matrix4x4f
{
public:

	float m[16];

    matrix4x4f()  { identity(); }

	matrix4x4f( float m0, float m4, float  m8, float m12,
               float m1, float m5, float  m9, float m13,
               float m2, float m6, float m10, float m14,
               float m3, float m7, float m11, float m15 );

	void identity(void);
	void translate(const vec3 &trans);
	void translate_x(const float &dist);
	void translate_y(const float &dist);
	void translate_z(const float &dist);
	void rotate(const float &angle, vec3 &axis);
	void rotate_x(const float &angle);
	void rotate_y(const float &angle);
	void rotate_z(const float &angle);
	void scale(const vec3 &scale);
    void transformPoint( vec3 *vec );
    void transformVector( vec3 *vec );

	void reflectm(const vec3 &n);

	void transposerotation();

    // Operators...
	matrix4x4f operator + (const matrix4x4f &other);
	matrix4x4f operator - (const matrix4x4f &other);
	matrix4x4f operator * (const matrix4x4f &other);
	vec3 operator*(const vec3 &v) const;

	matrix4x4f operator * (const float scalar);
};

#endif