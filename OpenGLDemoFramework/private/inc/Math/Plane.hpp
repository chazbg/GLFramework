#pragma once
#include "Vector.hpp"

class Plane2
{
public:
    Plane2(const float a, const float b, const float c);
    Vec2 normal;
    float offset;
};

class Plane3
{
public:
    Plane3(const float a, const float b, const float c, const float d);
    Vec3 normal;
    float offset;
};