#pragma once
#include "Math/Vector.hpp"
#include <iostream>
using namespace std;

void TestVec2()
{
	Vec2 v(2, 3);
	cout << v.toString() << endl;
	v.x = 5;
	cout << v.toString() << endl;
	v = 3 * v;
	cout << v.toString() << endl;
}

void TestVec3()
{
	Vec3 v(2, 3, 1);
	cout << v.toString() << endl;
	v.x = 5;
	cout << v.toString() << endl;
	v = 3 * v;
	cout << v.toString() << endl;
}