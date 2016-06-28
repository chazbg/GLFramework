#pragma once
#include "Math/Vector.hpp"

class Quarternion
{
public:
    Quarternion(const float x, const float y, const float z, const float w);
    Quarternion(const Vec4& v);
    Quarternion(const float r, const Vec3& v);
    Quarternion(const Quarternion& q);
    Quarternion operator*(const float scalar) const;
    friend Quarternion operator*(const float scalar, const Quarternion& rhs);
    Quarternion operator+(const Quarternion& rhs) const;
    Quarternion operator*(const Quarternion& rhs) const;
    float norm() const;
    Quarternion normalize() const;
    Vec3 rotate(const Vec3& v) const;
private:
    float data[4];
};