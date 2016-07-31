#include "Math/Matrix.hpp"

Matrix3::Matrix3()
{
    data[0] = 1.0f;
    data[1] = 0.0f;
    data[2] = 0.0f;

    data[3] = 0.0f;
    data[4] = 1.0f;
    data[5] = 0.0f;

    data[6] = 0.0f;
    data[7] = 0.0f;
    data[8] = 1.0f;
}

Matrix3::Matrix3(const Vec3 & a0, const Vec3 & a1, const Vec3 & a2)
{
    data[0] = a0.x;
    data[1] = a0.y;
    data[2] = a0.z;

    data[3] = a1.x;
    data[4] = a1.y;
    data[5] = a1.z;

    data[6] = a2.x;
    data[7] = a2.y;
    data[8] = a2.z;
}

Vec3 Matrix3::operator*(const Vec3 & vec) const
{
    float x = Vec3(data[0], data[1], data[2]).dot(vec);
    float y = Vec3(data[3], data[4], data[5]).dot(vec);
    float z = Vec3(data[6], data[7], data[8]).dot(vec);
    return Vec3(x, y, z);
}

Matrix3 Matrix3::operator*(const Matrix3 & rhs) const
{
    Vec3 a0 = Vec3(data[0], data[1], data[2]);
    Vec3 a1 = Vec3(data[3], data[4], data[5]);
    Vec3 a2 = Vec3(data[6], data[7], data[8]);

    Vec3 b0 = Vec3(rhs.data[0], rhs.data[1], rhs.data[2]);
    Vec3 b1 = Vec3(rhs.data[3], rhs.data[4], rhs.data[5]);
    Vec3 b2 = Vec3(rhs.data[6], rhs.data[7], rhs.data[8]);

    Vec3 c0 = a0.dot(b0) + a0.dot(b1) + a0.dot(b2);
    Vec3 c1 = a1.dot(b0) + a1.dot(b1) + a1.dot(b2);
    Vec3 c2 = a2.dot(b0) + a2.dot(b1) + a2.dot(b2);
         
    return Matrix3(c0, c1, c2);
}

Matrix3 Matrix3::operator*(const float rhs) const
{
    return Matrix3(Vec3(data[0] * rhs, data[1] * rhs, data[2] * rhs), 
                   Vec3(data[3] * rhs, data[4] * rhs, data[5] * rhs), 
                   Vec3(data[6] * rhs, data[7] * rhs, data[8] * rhs));
}

Matrix3 Matrix3::operator/(const float rhs) const
{
    return Matrix3(Vec3(data[0] / rhs, data[1] / rhs, data[2] / rhs),
                   Vec3(data[3] / rhs, data[4] / rhs, data[5] / rhs),
                   Vec3(data[6] / rhs, data[7] / rhs, data[8] / rhs));
}

Matrix3::operator string() const
{
    return "";
}

string Matrix3::toString() const
{
    return string();
}

Matrix4 Matrix3::toMatrix4() const
{
    return Matrix4(Vec4(data[0], data[1], data[2], 0),
                   Vec4(data[3], data[4], data[5], 0),
                   Vec4(data[6], data[7], data[8], 0), 
                   Vec4(0,       0,       0,       1));
}

const float * Matrix3::raw() const
{
    return data;
}

void Matrix3::setRotation(const float thetaX, const float thetaY, const float thetaZ)
{
        float sa = sin(thetaX);
        float ca = cos(thetaX);
        float sb = sin(thetaY);
        float cb = cos(thetaY);
        float sc = sin(thetaZ);
        float cc = cos(thetaZ);

        data[0] = cb * cc;
        data[1] = cc * sa * sb - ca * sc;
        data[2] = ca * cc * sb + sa * sc;

        data[3] = cb * sc;
        data[4] = ca * cc + sa * sb * sc;
        data[5] = -cc * sa + ca * sb * sc;

        data[6] = -sb;
        data[7] = cb * sa;
        data[8] = ca * cb;
}
