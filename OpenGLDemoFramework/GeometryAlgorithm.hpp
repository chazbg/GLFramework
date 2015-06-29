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
	static float Determinant(const Vec2& a, const Vec2& b, const Vec2& c);
	static std::vector<Vec2> Clip(const std::vector<Vec2>& inputPolygon, const std::vector<Vec2>& clippingPolygon);
	static Vec2 ComputeIntersection(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d);
	static bool ComputeIntersection2(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d, Vec2& intersectionPoint);
	static bool ComputeIntersection2(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d, Vec2& intersectionPoint, float& t, float& u);
	static bool ComputeIntersection3Perspective(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d, Vec3& fi, Vec3& fj);
	static bool ComputeIntersection3Parallel(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d, Vec3& fi, Vec3& fj);
	static std::vector<Vec2> SweepingLineIntersection(const std::vector<std::vector<Vec2>>& inputLines);
	static std::vector<std::vector<Vec2>> RotatingCalipers(const std::vector<Vec2>& inputPolygon);
	static std::vector<Vec2> IntersectPolygons(const std::vector<Vec2>& inputPolygon1, const std::vector<Vec2>& inputPolygon2);
	static std::vector<Vec2> TestVisibility(const std::vector<Vec2>& inputPolygon1, const std::vector<Vec2>& inputPolygon2);
};