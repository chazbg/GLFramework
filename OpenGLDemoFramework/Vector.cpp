#include "Vector.hpp"
#include <sstream>
#include <math.h>
//Vec2
Vec2::Vec2(const Vec2& vec)
{
	x = vec.x;
	y = vec.y;
}

Vec2& Vec2::operator=(const Vec2& vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2& Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2& Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Vec2 Vec2::operator*(const float scalar) const
{
	return Vec2(x * scalar, y * scalar);
}

Vec2& Vec2::operator*=(const float scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

Vec2 Vec2::operator/(const float scalar) const
{
	return Vec2(x / scalar, y / scalar);
}

Vec2& Vec2::operator/=(const float scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}

Vec2 operator*(const float scalar, const Vec2& rhs)
{
	return Vec2(scalar * rhs.x, scalar * rhs.y);
}

float Vec2::distanceTo(const Vec2& point)
{
	float dx = x - point.x;
	float dy = y - point.y;

	return sqrt(dx * dx + dy * dy);
}

float Vec2::distanceSquaredTo(const Vec2& point)
{
	float dx = x - point.x;
	float dy = y - point.y;

	return dx * dx + dy * dy;
}

Vec2::operator string()
{
	stringstream s;
	s << "(" << x << ", " << y << ")";
	return s.str();
}

string Vec2::toString()
{
	return this->operator string();
}

//Vec3
Vec3::Vec3(const Vec3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

Vec3& Vec3::operator=(const Vec3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

Vec3 Vec3::operator+(const Vec3& rhs) const
{
	return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vec3& Vec3::operator+=(const Vec3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

Vec3 Vec3::operator-(const Vec3& rhs) const
{
	return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vec3& Vec3::operator-=(const Vec3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

Vec3 Vec3::operator*(const float scalar) const
{
	return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3& Vec3::operator*=(const float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vec3 Vec3::operator/(const float scalar) const
{
	return Vec3(x / scalar, y / scalar, z / scalar);
}

Vec3& Vec3::operator/=(const float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

Vec3 Vec3::operator*(const Vec3& rhs) const
{
	return Vec3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
}

Vec3 operator*(const float scalar, const Vec3& rhs)
{
	return Vec3(scalar * rhs.x, scalar * rhs.y, scalar * rhs.z);
}

float Vec3::distanceTo(const Vec3& point)
{
	float dx = x - point.x;
	float dy = y - point.y;
	float dz = z - point.z;

	return sqrt(dx * dx + dy * dy + dz * dz);
}

float Vec3::distanceSquaredTo(const Vec3& point)
{
	float dx = x - point.x;
	float dy = y - point.y;
	float dz = z - point.z;

	return dx * dx + dy * dy + dz * dz;
}

Vec3::operator string()
{
	stringstream s;
	s << "(" << x << ", " << y << ", " << z << ")";
	return s.str();
}

string Vec3::toString()
{
	return this->operator string();
}

float* Vec3::raw()
{
	float* v = new float[3];
	v[0] = x;
	v[1] = y;
	v[2] = z;

	return v;
}

float Vec3::length() const
{
	return sqrt(x*x + y*y + z*z);
}
Vec3 Vec3::normalize() const
{
	float len = length();
	return Vec3(x/len, y/len, z/len);
}