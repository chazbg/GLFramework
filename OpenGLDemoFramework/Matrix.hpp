#pragma once

#include "Vector.hpp"
#include <iostream>
using namespace std;

class Matrix4 {
public:
	Matrix4(Vec4 a, Vec4 b, Vec4 c, Vec4 d) { m[0] = a; m[1] = b; m[2] = c; m[3] = d; }
	Vec4 operator*(const Vec4& vec)
	{
		Vec4 res;
		res.x = m[0].x * vec.x + m[0].y * vec.y + m[0].z * vec.z + m[0].w * vec.w;
		res.y = m[1].x * vec.x + m[1].y * vec.y + m[1].z * vec.z + m[1].w * vec.w;
		res.z = m[2].x * vec.x + m[2].y * vec.y + m[2].z * vec.z + m[2].w * vec.w;
		res.w = m[3].x * vec.x + m[3].y * vec.y + m[3].z * vec.z + m[3].w * vec.w;
		res /= res.w;
		return res;
	}
private:
	Vec4 m[4];
};