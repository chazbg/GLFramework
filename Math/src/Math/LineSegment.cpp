#include "Math/LineSegment.hpp"

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

LineSegment3::LineSegment3()
{
}

LineSegment3::LineSegment3(const Vec3& a, const Vec3& b) : a(a), b(b)
{

}

LineSegment3::operator string() const
{
    return "[" + a.toString() + ", " + b.toString() + "]";
}

string LineSegment3::toString() const
{
    return this->operator string();
}

bool LineSegment3::operator == (const LineSegment3& rhs) const
{
    return (a == rhs.a && b == rhs.b) || (a == rhs.b && b == rhs.a);
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