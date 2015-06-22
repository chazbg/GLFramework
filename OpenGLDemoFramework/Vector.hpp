#pragma once
#include <string>
using namespace std;

class Vec2
{
public:
	inline Vec2() {}
	inline Vec2(float x, float y) : x(x), y(y) {}
	inline ~Vec2() {}
	Vec2(const Vec2& vec);
	Vec2& operator=(const Vec2& vec);
	Vec2 operator+(const Vec2& rhs) const;
	Vec2& operator+=(const Vec2& rhs);
	Vec2 operator-(const Vec2& rhs) const;
	Vec2& operator-=(const Vec2& rhs);
	Vec2 operator*(const float scalar) const;
	Vec2& operator*=(const float scalar);
	Vec2 operator/(const float scalar) const;
	Vec2& operator/=(const float scalar);
	friend Vec2 operator*(const float scalar, const Vec2& rhs);
	float distanceTo(const Vec2& point);
	float distanceSquaredTo(const Vec2& point);
	operator string() const;
	string toString() const;
	bool operator==(const Vec2& rhs) const;
	bool operator!=(const Vec2& rhs) const;
	float dot(const Vec2& rhs) const;
	float perp(const Vec2& rhs) const;
	Vec2 toPolar() const;
	float polarAngle(const Vec2& rhs) const;

	//TODO: Add operations
public:
	float x;
	float y;
};

class Vec3
{
public:
	inline Vec3() {}
	inline Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	inline ~Vec3() {}
	Vec3(const Vec3& vec);
	Vec3& operator=(const Vec3& vec);
	Vec3 operator+(const Vec3& rhs) const;
	Vec3& operator+=(const Vec3& rhs);
	Vec3 operator-(const Vec3& rhs) const;
	Vec3& operator-=(const Vec3& rhs);
	Vec3 operator*(const float scalar) const;
	Vec3& operator*=(const float scalar);
	Vec3 operator/(const float scalar) const;
	Vec3& operator/=(const float scalar);
	Vec3 operator*(const Vec3& rhs) const;
	friend Vec3 operator*(const float scalar, const Vec3& rhs);
	float distanceTo(const Vec3& point);
	float distanceSquaredTo(const Vec3& point);
 	operator string() const;
	string toString() const;
	float* raw();
	float length() const;
	Vec3 normalize() const;
	//TODO: Add operations
public:
	float x;
	float y;
	float z;
};