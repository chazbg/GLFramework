#include "LineSegment.hpp"

LineSegment::LineSegment()
{
}

LineSegment::LineSegment(Vec2 a, Vec2 b) : a(a), b(b)
{

}

LineSegment::operator string() const
{
	return "[" + a.toString() + ", " + b.toString() + "]";
}

string LineSegment::toString() const
{
	return this->operator string();
}

OrderedLineSegment::OrderedLineSegment() : LineSegment()
{

}

OrderedLineSegment::OrderedLineSegment(Vec2 a, Vec2 b) : LineSegment(a, b)
{

}

bool OrderedLineSegment::operator==(const LineSegment& rhs) const 
{ 
	return  a == rhs.a && b == rhs.b; 
}

bool OrderedLineSegment::operator<(const LineSegment& rhs) const 
{ 
	return a.y < rhs.a.y; 
}