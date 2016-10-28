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

Quarternion operator*(const float scalar, const Quarternion& rhs)
{
    return rhs * scalar;
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

Quarternion Quarternion::inverse() const
{
    return Quarternion(data[0], -data[1], -data[2], -data[3]);
}

Vec3 Quarternion::rotate(const Vec3 & v) const
{
    Quarternion p(0.0f, v.x, v.y, v.z);
   
    Quarternion res = *this * p * inverse();
    return Vec3(res.data[1], res.data[2], res.data[3]);
}

Matrix3 Quarternion::toMatrix() const
{
    Quarternion q = normalize();
    float xy = 2.0f * q.data[1] * q.data[2];
    float zw = 2.0f * q.data[3] * q.data[0]; 
    float xz = 2.0f * q.data[1] * q.data[3];
    float yw = 2.0f * q.data[2] * q.data[0];
    float yz = 2.0f * q.data[2] * q.data[3];
    float xw = 2.0f * q.data[1] * q.data[0];

    float aa = 1.0f - 2.0f * (q.data[2] * q.data[2] + q.data[3] * q.data[3]);
    float bb = 1.0f - 2.0f * (q.data[1] * q.data[1] + q.data[3] * q.data[3]);
    float cc = 1.0f - 2.0f * (q.data[1] * q.data[1] + q.data[2] * q.data[2]);

    return Matrix3(
        Vec3(aa, xy - zw, xz + yw),
        Vec3(xy + zw, bb, yz - xw),
        Vec3(xz - yw, yz + xw, cc));
}

Quarternion Quarternion::slerp(const Quarternion & a, const Quarternion & b, float t)
{
    float cosPhi = a.data[0] * b.data[0] +
        a.data[1] * b.data[1] +
        a.data[2] * b.data[2] +
        a.data[3] * b.data[3];
    float phi = acos(cosPhi);
    float invSinPhi = 1.0f / sin(phi);
    float ca = sin((1.0f - t) * phi) * invSinPhi;
    float cb = sin(t * phi) * invSinPhi;

    return ca * a + cb * b;
}

Quarternion Quarternion::makeRotation(const Quarternion & q)
{
    Vec3 vectorPart = sin(q.data[0] / 2.0f) * Vec3(q.data[1], q.data[2], q.data[3]).normalize();
    return Quarternion(cos(q.data[0] / 2.0f), vectorPart.x, vectorPart.y, vectorPart.z).normalize();
}

Quarternion Quarternion::makeRotation(const float r, const Vec3 & v)
{
    Vec3 vectorPart = sin(r / 2.0f) * v.normalize();
    return Quarternion(cos(r / 2.0f), vectorPart.x, vectorPart.y, vectorPart.z).normalize();
}
