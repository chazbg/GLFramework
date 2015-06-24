#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "LineListMesh.hpp"
#include <iostream>

static LineListMesh* lines[5];

static void RenderScene()
{
	GLWrapper::ClearWindow();

	for (int i = 0; i < 5; i++)
	{
		lines[i]->Render();
	}

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

void DemoIntersectingLineSegments()
{
	GLUTWrapper::InitWindow(&RenderScene);
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
	
	for (int i = 0; i < intersections.size(); i++)
	{
		cout << "Intersection: " << intersections[i].toString() << endl;
	}
	/*for (int i = 0; i < 5; i++)
	{
		for (int j = i + 1; j < 5; j++)
		{
			Vec2 intersectionPoint;
			if (GeometryAlgorithm::ComputeIntersection2(lineSegments[i][0], lineSegments[i][1], lineSegments[j][0], lineSegments[j][1], intersectionPoint))
			{
				cout << "[" << lineSegments[i][0].toString() << ", " << lineSegments[i][1].toString() << "], [" << lineSegments[j][0].toString() << ", " << lineSegments[j][1].toString() << "] intersect in " << intersectionPoint.toString() << endl;
			}
			else
			{
				cout << "[" << lineSegments[i][0].toString() << ", " << lineSegments[i][1].toString() << "], [" << lineSegments[j][0].toString() << ", " << lineSegments[j][1].toString() << "] don't intersect" << endl;
			}
		}
	}*/
	for (int i = 0; i < 5; i++)
	{
		lines[i] = new LineListMesh(lineSegments[i]);
	}

	GLUTWrapper::RenderLoop();
}