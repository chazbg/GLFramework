#pragma once

#include "GeometryAlgorithm.hpp"
#include <cstdio>

void TestConvexHullGraham()
{
    std::vector<Vec2> testPoints;
    std::vector<Vec2> convexHull;
    //testPoints.push_back(Vec2(1, 4));
    //testPoints.push_back(Vec2(4, 2));
    //testPoints.push_back(Vec2(6, 10));
    //testPoints.push_back(Vec2(16, 5));
    //testPoints.push_back(Vec2(18, 7));
    //testPoints.push_back(Vec2(12, 16));
    //testPoints.push_back(Vec2(11, 3));
    //testPoints.push_back(Vec2(10, 7));

    testPoints.push_back(Vec2(1, 2));
    testPoints.push_back(Vec2(1.5, 3));
    testPoints.push_back(Vec2(2, 9));
    testPoints.push_back(Vec2(4, 1));
    testPoints.push_back(Vec2(4, 8));
    testPoints.push_back(Vec2(4, 10));
    testPoints.push_back(Vec2(6, 4));
    testPoints.push_back(Vec2(7.5, 6));

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

void TestComputeIntersection2()
{
    Vec2 segment1A(0, 0);
    Vec2 segment1B(1, 1);
    Vec2 segment2A(0, 1);
    Vec2 segment2B(1, 0);
    Vec2 intersectionPoint;

    bool res = GeometryAlgorithm::ComputeIntersection2(segment1A, segment1B, segment2A, segment2B, intersectionPoint);
    printf("TestComputeIntersection2 result : %d\n", res);
    if (res)
    {
        printf("Intersection point = %s\n", intersectionPoint.toString().c_str());
    }
}


void TestClip(std::vector<Vec2>& _inputPolygon, std::vector<Vec2>& _clippedPolygon, std::vector<Vec2>& _clippingPolygon)
{
    std::vector<Vec2> inputPolygon;
    std::vector<Vec2> clippingPolygon;
    std::vector<Vec2> outputPolygon;
    inputPolygon.push_back(Vec2(4, 1));
    inputPolygon.push_back(Vec2(7, 3));
    inputPolygon.push_back(Vec2(9, 15));
    inputPolygon.push_back(Vec2(15, 5));
    inputPolygon.push_back(Vec2(10, 6));
    inputPolygon.push_back(Vec2(7, 5));

    clippingPolygon.push_back(Vec2(3, 3));
    clippingPolygon.push_back(Vec2(6, 1));
    clippingPolygon.push_back(Vec2(11, 1));
    clippingPolygon.push_back(Vec2(13.5, 3));
    clippingPolygon.push_back(Vec2(8.5, 8.5));

    outputPolygon = GeometryAlgorithm::Clip(inputPolygon, clippingPolygon);
    printf("Sutherland-Hodgeman clipping points:\n");
    for (unsigned int i = 0; i < outputPolygon.size(); i++)
    {
        printf("%s\n", outputPolygon[i].toString().c_str());
    }

    _inputPolygon = inputPolygon;
    _clippedPolygon = outputPolygon;
    _clippingPolygon = clippingPolygon;
}

void TestRotatingCalipers()
{
    std::vector<Vec2> inputPolygon;
    std::vector<std::vector<Vec2>> antipodPoints;
    inputPolygon.push_back(Vec2(7, 4));
    inputPolygon.push_back(Vec2(10, 7));
    inputPolygon.push_back(Vec2(8, 10));
    inputPolygon.push_back(Vec2(4, 11));
    inputPolygon.push_back(Vec2(1, 5));
    inputPolygon.push_back(Vec2(2, 2));

    antipodPoints = GeometryAlgorithm::RotatingCalipers(inputPolygon);
}