#pragma once
#include "Vector.hpp"

class Line2
{
public:
    Line2(const Vec2& direction, const Vec2& origin);
    Vec2 direction;
    Vec2 origin;
};

class Line3
{
public:
    Line3(const Vec3& direction, const Vec3& origin);
    Vec3 direction;
    Vec3 origin;
};