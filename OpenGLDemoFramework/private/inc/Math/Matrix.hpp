#pragma once

#include "Vector.hpp"
#include <iostream>
#include <math.h>

using namespace std;

class Matrix4 {
public:
	Matrix4() 
	{
		m[0] = Vec4(1, 0, 0, 0);
		m[1] = Vec4(0, 1, 0, 0); 
		m[2] = Vec4(0, 0, 1, 0); 
		m[3] = Vec4(0, 0, 0, 1);

		for (int i = 0; i < 4; i++)
		{
			rawData[i * 4] = m[0].raw()[i];
			rawData[i * 4 + 1] = m[1].raw()[i];
			rawData[i * 4 + 2] = m[2].raw()[i];
			rawData[i * 4 + 3] = m[3].raw()[i];
		}
	}

	Matrix4(Vec4 a, Vec4 b, Vec4 c, Vec4 d) 
	{ 
		m[0] = a; 
		m[1] = b; 
		m[2] = c; 
		m[3] = d; 

		for (int i = 0; i < 4; i++)
		{
			rawData[i * 4] = m[0].raw()[i];
			rawData[i * 4 + 1] = m[1].raw()[i];
			rawData[i * 4 + 2] = m[2].raw()[i];
			rawData[i * 4 + 3] = m[3].raw()[i];
		}
	}

	~Matrix4()
	{
	}

	Vec4 operator*(const Vec4& vec) const
	{
		Vec4 res;
		res.x = m[0].dot(vec);
		res.y = m[1].dot(vec);
		res.z = m[2].dot(vec);
		res.w = m[3].dot(vec);
		res /= res.w;
		return res;
	}

	Matrix4 operator*(const Matrix4& rhs) const
	{
		Vec4 rhsColumn0(rhs.m[0].x, rhs.m[1].x, rhs.m[2].x, rhs.m[3].x);
		Vec4 rhsColumn1(rhs.m[0].y, rhs.m[1].y, rhs.m[2].y, rhs.m[3].y);
		Vec4 rhsColumn2(rhs.m[0].z, rhs.m[1].z, rhs.m[2].z, rhs.m[3].z);
		Vec4 rhsColumn3(rhs.m[0].w, rhs.m[1].w, rhs.m[2].w, rhs.m[3].w);
		Vec4 m0(m[0].dot(rhsColumn0), m[0].dot(rhsColumn1), m[0].dot(rhsColumn2), m[0].dot(rhsColumn3));
		Vec4 m1(m[1].dot(rhsColumn0), m[1].dot(rhsColumn1), m[1].dot(rhsColumn2), m[1].dot(rhsColumn3));
		Vec4 m2(m[2].dot(rhsColumn0), m[2].dot(rhsColumn1), m[2].dot(rhsColumn2), m[2].dot(rhsColumn3));
		Vec4 m3(m[3].dot(rhsColumn0), m[3].dot(rhsColumn1), m[3].dot(rhsColumn2), m[3].dot(rhsColumn3));
		return Matrix4(m0, m1, m2, m3);
	}

	Matrix4 operator*(const float rhs) const
	{
		return Matrix4(m[0] * rhs, m[1] * rhs, m[2] * rhs, m[3] * rhs);
	}

	Matrix4 operator/(const float rhs) const
	{
		return Matrix4(m[0] / rhs, m[1] / rhs, m[2] / rhs, m[3] / rhs);
	}

	operator string() const
	{
		return m[0].toString() + "\n" + m[1].toString() + "\n" + m[2].toString() + "\n" + m[3].toString() + "\n";
	}

	string toString() const
	{
		return this->operator std::string();
	}

	const float* raw()
	{
        for (int i = 0; i < 4; i++)
        {
            rawData[i * 4] = m[0].raw()[i];
            rawData[i * 4 + 1] = m[1].raw()[i];
            rawData[i * 4 + 2] = m[2].raw()[i];
            rawData[i * 4 + 3] = m[3].raw()[i];
        }

		return rawData;
	}

	void setTranslation(const Vec3& translation)
	{
		m[0].w = translation.x;
		m[1].w = translation.y;
		m[2].w = translation.z;
		cout << toString() << endl;
	}

    void setRotation(const float thetaX, const float thetaY, const float thetaZ)
    {
        float sa = sin(thetaX);
        float ca = cos(thetaX);
        float sb = sin(thetaY);
        float cb = cos(thetaY);
        float sc = sin(thetaZ);
        float cc = cos(thetaZ);
        m[0] = Vec4(cb * cc, cc * sa * sb - ca * sc, ca * cc * sb + sa * sc, m[0].w);
        m[1] = Vec4(cb * sc, ca * cc + sa * sb * sc, -cc * sa + ca * sb * sc, m[1].w);
        m[2] = Vec4(-sb, cb * sa, ca * cb, m[2].w);
    }
private:
	Vec4 m[4];
	float rawData[16];
};