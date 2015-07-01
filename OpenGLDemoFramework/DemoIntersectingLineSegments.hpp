#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "LineListMesh.hpp"
#include "PointListMesh.hpp"
#include <iostream>

namespace LIDemo
{

LineListMesh* lines[5];
PointListMesh* plm;

void RenderDemoIntersectingLineSegments()
{
	GLWrapper::ClearWindow();

	for (int i = 0; i < 5; i++)
	{
		lines[i]->Render();
	}

	plm->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

void DemoIntersectingLineSegments()
{
	GLUTWrapper::InitWindow(&RenderDemoIntersectingLineSegments);
	GLWrapper::InitRenderer();

	std::vector<std::vector<Vec2>> lineSegments;

	std::vector<Vec2> segment;
	segment.push_back(Vec2(3, 6));
	segment.push_back(Vec2(10, 4));
	lineSegments.push_back(segment);

	segment.clear();
	segment.push_back(Vec2(16, 10));
	segment.push_back(Vec2(16, 5));
	lineSegments.push_back(segment);

	segment.clear();
	segment.push_back(Vec2(1, 4));
	segment.push_back(Vec2(4, 2));
	lineSegments.push_back(segment);

	segment.clear();
	segment.push_back(Vec2(8, 1));
	segment.push_back(Vec2(18, 7));
	lineSegments.push_back(segment);

	segment.clear();
	segment.push_back(Vec2(12, 6));
	segment.push_back(Vec2(14, 3));
	lineSegments.push_back(segment);

	std::vector<Vec2> intersections = GeometryAlgorithm::SweepingLineIntersection(lineSegments);
	
	for (unsigned int i = 0; i < intersections.size(); i++)
	{
		cout << "Intersection: " << intersections[i].toString() << endl;
	}

	for (unsigned int i = 0; i < lineSegments.size(); i++)
	{
		lines[i] = new LineListMesh(lineSegments[i]);
	}

	plm = new PointListMesh(intersections, Vec3(0, 1, 0));

	GLUTWrapper::RenderLoop();
}

}