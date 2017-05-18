#pragma once
#include <string>

//TODO: remove "using namespace std;"
using namespace std;

class Vec2
{
public:
    inline Vec2() {}
    inline Vec2(float v) : x(v), y(v) {}
    inline Vec2(float x, float y) : x(x), y(y) {}
    inline ~Vec2() {}
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
    float length() const;
    Vec2 normalize() const;

    //TODO: Add operations
public:
    float x;
    float y;
};

class Vec3
{
public:
    inline Vec3() {}
    inline Vec3(float v) : x(v), y(v), z(v) {}
    inline Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    inline ~Vec3() {}
    Vec3& operator=(const Vec3& vec);
    Vec3 operator+(const Vec3& rhs) const;
    Vec3& operator+=(const Vec3& rhs);
    Vec3 operator-(const Vec3& rhs) const;
    Vec3& operator-=(const Vec3& rhs);
    Vec3 operator*(const float scalar) const;
    Vec3& operator*=(const float scalar);
    Vec3 operator/(const float scalar) const;
    Vec3& operator/=(const float scalar);
    friend Vec3 operator*(const float scalar, const Vec3& rhs);
    friend Vec3 operator-(const Vec3& rhs);
    bool operator==(const Vec3& rhs) const;
    float distanceTo(const Vec3& point);
    float distanceSquaredTo(const Vec3& point);
    operator string() const;
    string toString() const;
    const float* raw();
    float dot(const Vec3& rhs) const;
    Vec3 cross(const Vec3& rhs) const;
    float length() const;
    Vec3 normalize() const;
    //TODO: Add operations
public:
    float x;
    float y;
    float z;
};

class Vec4
{
public:
    inline Vec4() {}
    inline Vec4(float v) : x(v), y(v), z(v), w(v) {}
    inline Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    inline Vec4(const Vec3& v, const float w) : x(v.x), y(v.y), z(v.z), w(w) {}
    inline ~Vec4() {}
    Vec4& operator=(const Vec4& vec);
    Vec4 operator+(const Vec4& rhs) const;
    Vec4& operator+=(const Vec4& rhs);
    Vec4 operator-(const Vec4& rhs) const;
    Vec4& operator-=(const Vec4& rhs);
    Vec4 operator*(const float scalar) const;
    Vec4& operator*=(const float scalar);
    Vec4 operator/(const float scalar) const;
    Vec4& operator/=(const float scalar);
    friend Vec4 operator*(const float scalar, const Vec4& rhs);
    float distanceTo(const Vec4& point);
    float distanceSquaredTo(const Vec4& point);
    operator string() const;
    string toString() const;
    const float* raw();
    float dot(const Vec4& rhs) const;
    float length() const;
    Vec4 normalize() const;
    //TODO: Add operations
public:
    float x;
    float y;
    float z;
    float w;
};