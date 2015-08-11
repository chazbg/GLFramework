#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "LineListMesh.hpp"
#include "PointListMesh.hpp"

namespace GHSCDemo
{

PointListMesh* plm1;
LineListMesh* plm2;

void RenderDemoGrahamScan()
{
	GLWrapper::ClearWindow();

	plm1->Render();
	plm2->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

void DemoGrahamScan()
{
	std::vector<Vec2> inputPolygon, convexHull;

	inputPolygon.push_back(Vec2(1, 2));
	inputPolygon.push_back(Vec2(1.5, 3));
	inputPolygon.push_back(Vec2(2, 9));
	inputPolygon.push_back(Vec2(4, 1));
	inputPolygon.push_back(Vec2(4, 8));
	inputPolygon.push_back(Vec2(4, 10));
	inputPolygon.push_back(Vec2(6, 4));
	inputPolygon.push_back(Vec2(7.5, 6));

	convexHull = GeometryAlgorithm::ConvexHullGraham(inputPolygon);

	printf("ConvexHullGraham points:\n");
	for (unsigned int i = 0; i < convexHull.size(); i++)
	{
		printf("%s\n", convexHull[i].toString().c_str());
	}

	GLUTWrapper::InitWindow(&RenderDemoGrahamScan);
	GLWrapper::InitRenderer();

	plm1 = new PointListMesh(inputPolygon, Vec3(0, 1, 1), 6.0f);
	plm2 = new LineListMesh(convexHull, Vec3(1, 1, 0), 3.0f);

	GLUTWrapper::RenderLoop();
}

void DemoGrahamScan2()
{
	std::vector<Vec2> inputPolygon, convexHull;

	inputPolygon.push_back(Vec2(1, 4));
	inputPolygon.push_back(Vec2(4, 2));
	inputPolygon.push_back(Vec2(2, 9));
	inputPolygon.push_back(Vec2(4, 1));
	inputPolygon.push_back(Vec2(6, 10));
	inputPolygon.push_back(Vec2(16, 5));
	inputPolygon.push_back(Vec2(18, 7));
	inputPolygon.push_back(Vec2(7.5, 6));
	inputPolygon.push_back(Vec2(11, 3));
	inputPolygon.push_back(Vec2(10, 7));

	convexHull = GeometryAlgorithm::ConvexHullGraham(inputPolygon);

	printf("ConvexHullGraham points:\n");
	for (unsigned int i = 0; i < convexHull.size(); i++)
	{
		printf("%s\n", convexHull[i].toString().c_str());
	}

	GLUTWrapper::InitWindow(&RenderDemoGrahamScan);
	GLWrapper::InitRenderer();

	plm1 = new PointListMesh(inputPolygon, Vec3(0, 1, 1), 6.0f);
	plm2 = new LineListMesh(convexHull, Vec3(1, 1, 0), 3.0f);

	GLUTWrapper::RenderLoop();
}

}