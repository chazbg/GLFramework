#include "GeometryAlgorithm.hpp"
#include <cstdio>

void TestConvexHullGraham()
{
	std::vector<Vec2> testPoints;
	std::vector<Vec2> convexHull;
	testPoints.push_back(Vec2(0.2f, 0.5f));
	testPoints.push_back(Vec2(0.5f, 0.5f));
	testPoints.push_back(Vec2(0.5f, 0.7f));
	testPoints.push_back(Vec2(-1.2f, 0.0f));
	testPoints.push_back(Vec2(-1.2f, 0.5f));
	testPoints.push_back(Vec2(0.0f, 0.0f));
	testPoints.push_back(Vec2(-1.3f, 0.0f));

	convexHull = GeometryAlgorithm::ConvexHullGraham(testPoints);
	printf("ConvexHullGraham points:\n");
	for (unsigned int i = 0; i < convexHull.size(); i++)
	{
		printf("%s\n", convexHull[i].toString().c_str());
	}
	
}

void TestPointInPolygon()
{
	Vec2 testPoint(0.2f, 0.5f);
	Vec2 testPoint2(0.5f, 0.5f);
	std::vector<Vec2> testPoints;
	std::vector<Vec2> polygon;
	bool res;

	polygon.push_back(Vec2(0, 0));
	polygon.push_back(Vec2(1, 0));
	polygon.push_back(Vec2(0, 1));

	testPoints.push_back(Vec2(0.2f, 0.5f));
	testPoints.push_back(Vec2(0.5f, 0.5f));
	testPoints.push_back(Vec2(0.5f, 0.7f));
	testPoints.push_back(Vec2(-1.2f, 0.5f));
	testPoints.push_back(Vec2(0.0f, 0.0f));

	for (unsigned int i = 0; i < testPoints.size(); i++)
	{
		res = GeometryAlgorithm::PointInPolygon(testPoints[i], polygon);
		printf("TestPointInPolygon result for point: %s -> %d\n", testPoints[i].toString().c_str(), res);
	}

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