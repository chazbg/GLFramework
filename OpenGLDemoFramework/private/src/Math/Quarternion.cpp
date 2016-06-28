#include "Math/Quarternion.hpp"
#include <math.h>

Quarternion::Quarternion(const float x, const float y, const float z, const float w)
{
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;
}

Quarternion::Quarternion(const Vec4 & v)
{
    data[0] = v.x;
    data[1] = v.y;
    data[2] = v.z;
    data[3] = v.w;
}

Quarternion::Quarternion(const float r, const Vec3 & v)
{
    data[0] = r;
    data[1] = v.x;
    data[2] = v.y;
    data[3] = v.z;
}

Quarternion::Quarternion(const Quarternion & q)
{
    data[0] = q.data[0];
    data[1] = q.data[1];
    data[2] = q.data[2];
    data[3] = q.data[3];
}

Quarternion Quarternion::operator*(const float scalar) const
{
    return Quarternion(
        data[0] * scalar,
        data[1] * scalar,
        data[2] * scalar,
        data[3] * scalar);
}

Quarternion Quarternion::operator+(const Quarternion & rhs) const
{
    return Quarternion(
        data[0] + rhs.data[0],
        data[1] + rhs.data[1],
        data[2] + rhs.data[2],
        data[3] + rhs.data[3]);
}

Quarternion Quarternion::operator*(const Quarternion & rhs) const
{
    return Quarternion(
        data[0] * rhs.data[0] - data[1] * rhs.data[1] - data[2] * rhs.data[2] - data[3] * rhs.data[3],
        data[0] * rhs.data[1] + data[1] * rhs.data[0] + data[2] * rhs.data[3] - data[3] * rhs.data[2],
        data[0] * rhs.data[2] - data[1] * rhs.data[3] + data[2] * rhs.data[0] + data[3] * rhs.data[1],
        data[0] * rhs.data[3] + data[1] * rhs.data[2] - data[2] * rhs.data[1] + data[3] * rhs.data[0]);
}

float Quarternion::norm() const
{
    return sqrt(
        data[0] * data[0] + 
        data[1] * data[1] + 
        data[2] * data[2] + 
        data[3] * data[3]);
}

Quarternion Quarternion::normalize() const
{
    float n = norm();

    if (n == 0)
    {
        return *this;
    }
    else
    {
        return Quarternion(
            data[0] / n,
            data[1] / n,
            data[2] / n,
            data[3] / n);
    }    
}

Vec3 Quarternion::rotate(const Vec3 & v) const
{
    Quarternion p(0.0f, v);
    float halfTheta    = data[0] / 2.0f;
    float cosHalfTheta = cos(halfTheta);
    float sinHalfTheta = sin(halfTheta);
    Vec3 vectorPart    = sinHalfTheta * Vec3(data[1], data[2], data[3]).normalize();
   
    Quarternion q(cosHalfTheta, vectorPart);
    Quarternion qInv(cosHalfTheta, -vectorPart);
    Quarternion res = q * p * qInv;
    return Vec3(res.data[1], res.data[2], res.data[3]);
}
