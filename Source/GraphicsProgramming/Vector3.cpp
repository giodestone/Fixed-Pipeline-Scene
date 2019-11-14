#include "Vector3.h"

Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 Vector3::copy() {
	Vector3 copy(
		this->x,
		this->y,
		this->z);
	return copy;
}

bool Vector3::Equals(const Vector3& v2, float epsilon) const {
	return ((fabsf(this->x - v2.x) < epsilon) &&
		(fabsf(this->y - v2.y) < epsilon) &&
		(fabsf(this->z - v2.z) < epsilon));
}

bool Vector3::Equals(const Vector3& v2) const
{
	return Equals(v2, 0.00001f);
}


float Vector3::length() {
	return (float)sqrt(this->lengthSquared());
}

float Vector3::lengthSquared() {
	return (
		this->x*this->x +
		this->y*this->y +
		this->z*this->z
		);
}

void Vector3::Normalise() {
	float mag = this->length();
	if (mag) {
		float multiplier = 1.0f / mag;
		this->x *= multiplier;
		this->y *= multiplier;
		this->z *= multiplier;
	}
}

Vector3 Vector3::Normalised()
{
	Vector3 norm(x, y, z);
	norm.Normalise();
	return norm;
}

Vector3 Vector3::Cross(const Vector3& v2) {
	Vector3 cross(
		(this->y * v2.z - this->z * v2.y),
		(this->z * v2.x - this->x * v2.z),
		(this->x * v2.y - this->y * v2.x)
		);
	return cross;
}

void Vector3::Subtract(const Vector3& v1, float scale) {
	this->x -= (v1.x*scale);
	this->y -= (v1.y*scale);
	this->z -= (v1.z*scale);
}

void Vector3::Set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3::SetX(float x) {
	this->x = x;
}

void Vector3::SetY(float y) {
	this->y = y;
}

void Vector3::SetZ(float z) {
	this->z = z;
}

float Vector3::GetX() const {
	return this->x;
}

float Vector3::GetY() const {
	return this->y;
}

float Vector3::GetZ() const {
	return this->z;
}

std::array<GLfloat, 4> Vector3::GetVector4v() const
{
	std::array<GLfloat, 4> vector4v = { static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(z), 1.f };
	return vector4v;
}

float Vector3::Dot(const Vector3& v2) {
	return (this->x*v2.x +
		this->y*v2.y +
		this->z*v2.z
		);
}

void Vector3::Scale(float scale) {
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
}

void Vector3::Add(const Vector3& v1, float scale) {
	this->x += (v1.x*scale);
	this->y += (v1.y*scale);
	this->z += (v1.z*scale);
}

Vector3 Vector3::operator+(const Vector3& v2) {
	return Vector3(this->x + v2.x, this->y + v2.y, this->z + v2.z);
}

Vector3 Vector3::operator-(const Vector3& v2) {
	return Vector3(this->x - v2.x, this->y - v2.y, this->z - v2.z);
}

Vector3 Vector3::operator*(const float rhs)
{
	return Vector3(this->x * rhs, this->y * rhs, this->z * rhs);
}

Vector3& Vector3::operator+=(const Vector3& v2) {
	this->x += v2.x;
	this->y += v2.y;
	this->z += v2.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v2) {
	this->x -= v2.x;
	this->y -= v2.y;
	this->z -= v2.z;
	return *this;
}

Vector3 & Vector3::operator*=(const float rhs)
{
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;

	return *this;
}
