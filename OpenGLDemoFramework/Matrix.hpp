#pragma once

#include "Vector.hpp"
#include <iostream>
using namespace std;

class Matrix4 {
public:
	Matrix4() { m[0] = Vec4(0, 0, 0, 0); m[1] = Vec4(0, 0, 0, 0); m[2] = Vec4(0, 0, 0, 0); m[3] = Vec4(0, 0, 0, 0); }
	Matrix4(Vec4 a, Vec4 b, Vec4 c, Vec4 d) { m[0] = a; m[1] = b; m[2] = c; m[3] = d; }
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
private:
	Vec4 m[4];
};