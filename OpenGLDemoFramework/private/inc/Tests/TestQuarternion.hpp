#pragma once
#include "Math/Quarternion.hpp"
#include <math.h>
#include <iostream>
#include <assert.h>

#define M_PI static_cast<float>(acos(-1.0))
#define EPS 0.000001f

void TestQuarternion1()
{
    Vec3 v(1.0f, 0.0f, 0.0f);
    Quarternion rot(M_PI / 2.0f, Vec3(0.0f, 1.0f, 0.0f));
    Vec3 res = rot.rotate(v);
    std::cout << res.toString() << std::endl;
    assert(res.distanceTo(Vec3(0.0f, 0.0f, -1.0f)) < EPS);
}