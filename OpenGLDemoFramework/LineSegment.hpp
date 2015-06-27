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

class OrderedLineSegment : public LineSegment
{
public:
	OrderedLineSegment();
	OrderedLineSegment(Vec2 a, Vec2 b);
	bool operator==(const LineSegment& rhs) const;
	bool operator<(const LineSegment& rhs) const;
};