#include "GeometryAlgorithm.hpp"
#include <cstdio>
#include <cmath>

#define EPS 0.0001f

std::vector<Vec2> GeometryAlgorithm::ConvexHull(const std::vector<Vec2>& inputs)
{
	return inputs;
}

bool GeometryAlgorithm::PointInPolygon(const Vec2& inputPoint, const std::vector<Vec2>& inputPolygon)
{
	int l = 0;
	int i = 0;
	bool res = false;
	Vec2 lineA(-1, inputPoint.y);
	Vec2 lineB(0, inputPoint.y);

	for (unsigned int i = 0; i < inputPolygon.size(); i++)
	{
		const Vec2& a = inputPolygon[i];
		const Vec2& b = inputPolygon[(i + 1) % inputPolygon.size()];

		if (PointInLine(inputPoint, a, b))
		{
			res = true;
			printf("InputPoint is a contour point\n");
			break;
		}
		else if (a.y != b.y)
		{
			Vec2 intersectionPoint;
			if (LineIntersection(lineA, lineB, a, b, intersectionPoint) && intersectionPoint.x < inputPoint.x)
			{
				l++;
			}
		}
	}

	if (l % 2 == 1)
	{
		res = true;
	}

	return res;
}

bool GeometryAlgorithm::PointInLine(const Vec2& inputPoint, const Vec2& lineEndA, const Vec2& lineEndB)
{
	float t;
	bool res = false;
	//lineEndA * t + lineEndB * (1 - t) = inputPoint
	//lineEndA * t + lineEndB  - lineEndB * t = inputPoint
	//(lineEndA - lineEndB) * t = inputPoint - lineEndB
	//x = (inputPoint - lineEndB) / (lineEndA - lineEndB)
	if (inputPoint.x != lineEndB.x)
	{
		t = (inputPoint.x - lineEndB.x) / (lineEndA.x - lineEndB.x);
		printf("t = %f\n", t);

		if (t >= 0.0f && t <= 1.0f)
		{
			if (lineEndA.y * t + lineEndB.y * (1 - t) == inputPoint.y)
			{
				res = true;
			}
		}
	}
	else if (inputPoint.y != lineEndB.y)
	{
		t = (inputPoint.y - lineEndB.y) / (lineEndA.y - lineEndB.y);
		printf("t = %f\n", t);

		if (t >= 0.0f && t <= 1.0f)
		{
			res = true;
		}
	}

	return res;
}

bool GeometryAlgorithm::LineIntersection(const Vec2& rayPointA, 
	const Vec2& rayPointB, 
	const Vec2& segmentEndA, 
	const Vec2& segmentEndB,
	Vec2& intersectionPoint)
{
	bool res = false;
	Vec2 u = rayPointB - rayPointA;
	Vec2 v = segmentEndB - segmentEndA;
	Vec2 w = rayPointA - segmentEndA;
	float d = u.perp(v);

	if (abs(d) >= EPS)
	{
		float tI = u.perp(w) / d;
		
		if (tI >= 0.0f && tI <= 1.0f)
		{
			res = true;
			intersectionPoint = segmentEndA + tI * v;
		}
	}

	return res;
}