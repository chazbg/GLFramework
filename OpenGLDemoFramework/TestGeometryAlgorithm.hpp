#include "GeometryAlgorithm.hpp"
#include <cstdio>

void TestPointInPolygon()
{
	Vec2 testPoint(0.2, 0.5);
	std::vector<Vec2> polygon;
	bool res;

	polygon.push_back(Vec2(0, 0));
	polygon.push_back(Vec2(1, 0));
	polygon.push_back(Vec2(0, 1));

	res = GeometryAlgorithm::PointInPolygon(testPoint, polygon);
	printf("TestPointInPolygon result : %d\n", res);
}

void TestPointInLine()
{
	Vec2 a(0, 0);
	Vec2 b(1, 1);
	Vec2 testPoint(0.5f, 0.7f);

	bool res = GeometryAlgorithm::PointInLine(testPoint, a, b);
	printf("TestPointInLine result : %d\n", res);
}

void TestLineIntersection()
{
	Vec2 rayA(-1, 0.5);
	Vec2 rayB(0, 0.5);
	Vec2 segmentA(0,0);
	Vec2 segmentB(1, 1);
	Vec2 intersectionPoint;

	bool res = GeometryAlgorithm::LineIntersection(rayA, rayB, segmentA, segmentB, intersectionPoint);
	printf("TestLineIntersection result : %d\n", res);
	if (res)
	{
		printf("Intersection point = %s\n", intersectionPoint.toString().c_str());
	}
}