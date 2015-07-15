#pragma once
#include "Matrix.hpp"

void TestMat4()
{
	Matrix4 id(
		Vec4(1, 0, 0, 0),
		Vec4(0, 1, 0, 0),
		Vec4(0, 0, 1, 0),
		Vec4(0, 0, 0, 1)
		);

	Matrix4 m1(
		Vec4(2, 2, 0, 0),
		Vec4(0, 1, 2, 0),
		Vec4(0, 0, 1, 3),
		Vec4(0, 0, 0, 1)
		);

	Matrix4 m2(
		Vec4(1, 0, 0, 2),
		Vec4(0, 1, 0, 2),
		Vec4(0, 0, 1, 2),
		Vec4(0, 0, 0, 1)

		);
	cout << (m1 * m2).toString() << endl;

}