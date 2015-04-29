#include "Plane.hpp"

//Plane2
Plane2::Plane2(const float a, const float b, const float c) :
normal(a, b), offset(c)
{
}

//Plane3
Plane3::Plane3(const float a, const float b, const float c, const float d) :
normal(a, b, c), offset(d)
{
}