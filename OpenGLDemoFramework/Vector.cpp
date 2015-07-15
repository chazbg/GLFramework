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

Vec2::operator string() const
{
	stringstream s;
	s << "(" << x << ", " << y << ")";
	return s.str();
}

string Vec2::toString() const
{
	return this->operator string();
}

bool Vec2::operator==(const Vec2& rhs) const
{
	return x == rhs.x && y == rhs.y;
}

bool Vec2::operator!=(const Vec2& rhs) const
{
	return !((*this) == rhs);
}

float Vec2::dot(const Vec2& rhs) const
{
	return x * rhs.x + y * rhs.y;
}

float Vec2::perp(const Vec2& rhs) const
{
	return x * rhs.y - y * rhs.x;
}

Vec2 Vec2::toPolar() const
{
	return Vec2(sqrt(x*x + y*y), atan2(y, x));
}

float Vec2::polarAngle(const Vec2& rhs) const
{
	return atan2(rhs.y - y, rhs.x - x);
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

bool Vec3::operator== (const Vec3& rhs) const
{
	return x == rhs.x && y == rhs.y && z == rhs.z;
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

Vec3::operator string() const
{
	stringstream s;
	s << "(" << x << ", " << y << ", " << z << ")";
	return s.str();
}

string Vec3::toString() const
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

float Vec3::dot(const Vec3& rhs) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z;
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

//Vec4
Vec4::Vec4(const Vec4& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

Vec4& Vec4::operator=(const Vec4& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;

	return *this;
}

Vec4 Vec4::operator+(const Vec4& rhs) const
{
	return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

Vec4& Vec4::operator+=(const Vec4& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

Vec4 Vec4::operator-(const Vec4& rhs) const
{
	return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

Vec4& Vec4::operator-=(const Vec4& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;

	return *this;
}

Vec4 Vec4::operator*(const float scalar) const
{
	return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vec4& Vec4::operator*=(const float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;

	return *this;
}

Vec4 Vec4::operator/(const float scalar) const
{
	return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
}

Vec4& Vec4::operator/=(const float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;

	return *this;
}

Vec4 operator*(const float scalar, const Vec4& rhs)
{
	return Vec4(scalar * rhs.x, scalar * rhs.y, scalar * rhs.z, scalar * rhs.w);
}

float Vec4::distanceTo(const Vec4& point)
{
	float dx = x - point.x;
	float dy = y - point.y;
	float dz = z - point.z;
	float dw = w - point.w;

	return sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

float Vec4::distanceSquaredTo(const Vec4& point)
{
	float dx = x - point.x;
	float dy = y - point.y;
	float dz = z - point.z;
	float dw = w - point.w;

	return dx * dx + dy * dy + dz * dz + dw * dw;
}

Vec4::operator string() const
{
	stringstream s;
	s << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	return s.str();
}

string Vec4::toString() const
{
	return this->operator string();
}

float* Vec4::raw()
{
	float* v = new float[4];
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;

	return v;
}

float Vec4::dot(const Vec4& rhs) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

float Vec4::length() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

Vec4 Vec4::normalize() const
{
	float len = length();
	return Vec4(x / len, y / len, z / len, w / len);
}