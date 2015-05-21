#pragma once

#include "Vector.hpp"
#include <vector>

class GeometryAlgorithm
{
public:
	static std::vector<Vec2> ConvexHull(const std::vector<Vec2>& inputs);
	static std::vector<Vec2> ConvexHullGraham(const std::vector<Vec2>& inputs);
	static bool PointInPolygon(const Vec2& inputPoint, const std::vector<Vec2>& inputPolygon);
	static bool PointInLine(const Vec2& inputPoint, const Vec2& lineEndA, const Vec2& lineEndB);
	static bool LineIntersection(const Vec2& rayPointA, const Vec2& rayPointB, const Vec2& segmentEndA, const Vec2& segmentEndB, Vec2& intersectionPoint);
	static float TriangleArea(const Vec2& a, const Vec2& b, const Vec2& c);
};