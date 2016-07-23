#pragma once
#include "Math/Vector.hpp"
#include <iostream>
#include <assert.h>
#include <math.h>
using namespace std;

#define EPS 0.00000001f
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
    assert(v.raw()[0] == 15);
    assert(v.raw()[1] == 9);
    assert(v.raw()[2] == 3);

    v = v / 3;

    assert(v.raw()[0] == 5);
    assert(v.raw()[1] == 3);
    assert(v.raw()[2] == 1);

    v *= 1.5;

    assert(abs(v.raw()[0] - 7.5) < EPS);
    assert(abs(v.raw()[1] - 4.5) < EPS);
    assert(abs(v.raw()[2] - 1.5) < EPS);
}

void TestVec4()
{
    Vec4 v(2, 3, 1, 4);
    cout << v.toString() << endl;
    v.x = 5;
    cout << v.toString() << endl;
    v = 3 * v;
    cout << v.toString() << endl;
    assert(v.raw()[0] == 15);
    assert(v.raw()[1] == 9);
    assert(v.raw()[2] == 3);
    assert(v.raw()[3] == 12);

    v = v / 3;

    assert(v.raw()[0] == 5);
    assert(v.raw()[1] == 3);
    assert(v.raw()[2] == 1);
    assert(v.raw()[3] == 4);

    v *= 1.5;

    assert(abs(v.raw()[0] - 7.5) < EPS);
    assert(abs(v.raw()[1] - 4.5) < EPS);
    assert(abs(v.raw()[2] - 1.5) < EPS);
    assert(abs(v.raw()[3] - 6.0) < EPS);
}