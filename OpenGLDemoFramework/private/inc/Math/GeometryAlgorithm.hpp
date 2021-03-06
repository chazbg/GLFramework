#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
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
    static bool ComputePerspectiveIntersection(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d, Vec3& fi, Vec3& fj, const Matrix4& mat, const float projCenterZ);
    static bool ComputeParallelIntersection(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d, Vec3& fi, Vec3& fj, const Matrix4& mat);
    static std::vector<Vec2> SweepingLineIntersection(const std::vector<std::vector<Vec2>>& inputLines);
    static std::vector<std::vector<Vec2>> RotatingCalipers(const std::vector<Vec2>& inputPolygon);
    static std::vector<Vec2> IntersectPolygons(const std::vector<Vec2>& inputPolygon1, const std::vector<Vec2>& inputPolygon2);
    static std::vector<Vec2> TestVisibility(const std::vector<Vec2>& inputPolygon1, const std::vector<Vec2>& inputPolygon2);
    static std::vector<Vec2> ProjectPointList(const std::vector<Vec3>& points, const Matrix4& mat);
    static Matrix4 CreatePerspectiveMatrix(const float angleOfView, const float aspectRatio, const float zNear, const float zFar);
    static Matrix4 CreateLookAtMatrix(const Vec3& cameraPosition, const Vec3& cameraTarget, const Vec3& cameraUpVector);
    static Matrix4 CreateSRTMatrix(const Vec3& scale, const Vec3& rotationInRadians, const Vec3& translation);
private:
    
};