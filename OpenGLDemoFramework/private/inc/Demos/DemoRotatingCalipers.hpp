#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "LineListMesh.hpp"
#include "PointListMesh.hpp"
#include <iostream>

namespace RCDemo
{

LineListMesh* llm;
LineListMesh* maxDistance;
std::vector<PointListMesh*> plm;
int time = 0;

void RenderDemoRotatingCalipers()
{
	GLWrapper::ClearWindow();

	llm->Render();
	maxDistance->Render();

	plm[((int)(time * 0.01)) % plm.size()]->Render();
	time++;

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

void DemoRotatingCalipers()
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

	GLUTWrapper::InitWindow(&RenderDemoRotatingCalipers);
	GLWrapper::InitRenderer();

	llm = new LineListMesh(inputPolygon, Vec3(1, 1, 1), 4.0f);
	std::vector<Vec2> maxDist = antipodPoints[0];
	for (unsigned int i = 0; i < antipodPoints.size(); i++)
	{
		float dist = antipodPoints[i][0].distanceTo(antipodPoints[i][1]);
		if (dist > maxDist[0].distanceTo(maxDist[1]))
		{
			maxDist = antipodPoints[i];
		}
	}

	std::cout << "Pairs: " << antipodPoints.size() << std::endl;
	std::cout << "Max Distance points: " << maxDist[0].toString() << ", " << maxDist[1].toString() << std::endl;
	std::cout << "Max Distance: " << maxDist[0].distanceTo(maxDist[1]) << std::endl;
	maxDistance = new LineListMesh(maxDist, Vec3(1, 1, 0), 2.0f);
	for (unsigned int i = 0; i < antipodPoints.size(); i++)
	{
		plm.push_back(new PointListMesh(antipodPoints[i], Vec3(0.7f, 0, 0), 10.0f));
	}
	

	GLUTWrapper::RenderLoop();
}

}