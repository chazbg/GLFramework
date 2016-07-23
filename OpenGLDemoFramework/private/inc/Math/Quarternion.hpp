#pragma once
#include "Math/Vector.hpp"

class Quarternion
{
public:
    Quarternion(const float x, const float y, const float z, const float w);
    Quarternion(const Vec4& v);
    Quarternion(const Quarternion& q);
    Quarternion operator*(const float scalar) const;
    friend Quarternion operator*(const float scalar, const Quarternion& rhs);
    Quarternion operator+(const Quarternion& rhs) const;
    Quarternion operator*(const Quarternion& rhs) const;
    float norm() const;
    Quarternion normalize() const;
    Quarternion inverse() const;
    Vec3 rotate(const Vec3& v) const;
    static Quarternion slerp(const Quarternion& a, const Quarternion& b, float t);
    static Quarternion makeRotation(const Quarternion& q);
    static Quarternion makeRotation(const float r, const Vec3& v);
private:
    
    float data[4];
};

