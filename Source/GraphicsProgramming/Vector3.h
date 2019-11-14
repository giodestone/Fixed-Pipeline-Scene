#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


#include <array>
#include <math.h>

class Vector3 {

public:
	float x;
	float y;
	float z;

	Vector3(float x = 0, float y = 0, float z = 0);
	Vector3 copy();

	//Getters and Setters
	void Set(float x, float y, float z);
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);

	float GetX() const;
	float GetY() const;
	float GetZ() const;

	std::array<GLfloat, 4> GetVector4v() const;

	//Operations

	void Add(const Vector3& v1, float scale = 1.0);
	void Subtract(const Vector3& v1, float scale = 1.0);
	void Scale(float scale);

	float Dot(const Vector3& v2); //this dots the vector by a vector and returns the dot product
	Vector3 Cross(const Vector3& v2); //This crosses the vector by the value.

	void Normalise();
	Vector3 Normalised();
	float length();
	float lengthSquared();

	bool Equals(const Vector3& v2, float epsilon) const;
	bool Equals(const Vector3& v2) const;

	//Operators

	Vector3 operator+(const Vector3& v2);
	Vector3 operator-(const Vector3& v2);
	Vector3 operator*(const float rhs);

	Vector3& operator+=(const Vector3& v2);
	Vector3& operator-=(const Vector3& v2);
	Vector3& operator*=(const float rhs);
};

#endif