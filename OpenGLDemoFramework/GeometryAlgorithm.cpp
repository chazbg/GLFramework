#include "GeometryAlgorithm.hpp"
#include <cstdio>
#include <cmath>
#include <algorithm>

#define EPS 0.0001f

std::vector<Vec2> GeometryAlgorithm::ConvexHull(const std::vector<Vec2>& inputs)
{
	return inputs;
}

std::vector<Vec2> GeometryAlgorithm::ConvexHullGraham(const std::vector<Vec2>& inputs)
{
	std::vector<Vec2> convexHull;
	std::vector<Vec2> sortedInputs = inputs;
	Vec2 startingPoint = inputs[0];

	int k = 2;
	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		if ((inputs[i].y < startingPoint.y) || (inputs[i].y == startingPoint.y && inputs[i].x < startingPoint.x))
		{
			startingPoint = inputs[i];
			std::iter_swap(sortedInputs.begin() + i, sortedInputs.begin() + 1);
		}
	}

	struct less_than_key
	{
		inline bool operator() (const Vec2& u, const Vec2& v)
		{
			return u.toPolar().y < v.toPolar().y;
		}
	};

	std::sort(sortedInputs.begin() + 1, sortedInputs.end(), less_than_key());

	printf("Starting point: %s\n", startingPoint.toString().c_str());
	
	int M = 1;
	for (unsigned int i = 1; i < sortedInputs.size(); i++)
	{
		while (TriangleArea(sortedInputs[M - 1], sortedInputs[M], sortedInputs[i]) <= 0)
		{
			if (M > 1)
			{
				M--;
			}
			else if (i == sortedInputs.size())
			{
				break;
			}
			else
			{
				i++;
			}
		}

		M++;

	}

	return sortedInputs;
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
			printf("%s is a contour point\n", inputPoint.toString().c_str());
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

	if (lineEndA.x == lineEndB.x)
	{
		if (inputPoint.x == lineEndA.x)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (lineEndA.y == lineEndB.y)
	{
		if (inputPoint.y == lineEndA.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (inputPoint.x != lineEndB.x)
	{
		t = (inputPoint.x - lineEndB.x) / (lineEndA.x - lineEndB.x);
		printf("%s, %s, %s\n", inputPoint.toString().c_str(), lineEndA.toString().c_str(), lineEndB.toString().c_str());
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
		printf("%s, %s, %s\n", inputPoint.toString().c_str(), lineEndA.toString().c_str(), lineEndB.toString().c_str());
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

float GeometryAlgorithm::TriangleArea(const Vec2& a, const Vec2& b, const Vec2& c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}