#pragma once

#include "Vector.hpp"

class LineSegment
{
public:
    LineSegment();
    LineSegment(Vec2 a, Vec2 b);
    operator string() const;
    string toString() const;

    Vec2 a;
    Vec2 b;
};

class LineSegment3
{
public:
    LineSegment3();
    LineSegment3(const Vec3& a, const Vec3& b);
    operator string() const;
    string toString() const;
    bool operator==(const LineSegment3& rhs) const;

    Vec3 a;
    Vec3 b;
};

class OrderedLineSegment : public LineSegment
{
public:
    OrderedLineSegment();
    OrderedLineSegment(Vec2 a, Vec2 b);
    bool operator==(const LineSegment& rhs) const;
    bool operator<(const LineSegment& rhs) const;
};