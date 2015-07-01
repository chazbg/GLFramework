#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "TestGeometryAlgorithm.hpp"
#include "LineListMesh.hpp"

static LineListMesh* plm1;
static PointListMesh* plm2;
static PointListMesh* plm3;

static void RenderDemoPointInPolygon()
{
	GLWrapper::ClearWindow();

	plm1->Render();
	plm2->Render();
	plm3->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

void DemoPointInPolygon()
{
	std::vector<Vec2> testPoints;
	std::vector<Vec2> polygon;
	std::vector<Vec2> pointsInside;
	std::vector<Vec2> pointsOutside;
	bool res;

	polygon.push_back(Vec2(1, 1));
	polygon.push_back(Vec2(3, 5));
	polygon.push_back(Vec2(5, 4));
	polygon.push_back(Vec2(9, 7));
	polygon.push_back(Vec2(11, 3));
	polygon.push_back(Vec2(11, 1));
	polygon.push_back(Vec2(8, 2));

	testPoints.push_back(Vec2(4, 2));
	testPoints.push_back(Vec2(10, 3.5));

	for (unsigned int i = 0; i < testPoints.size(); i++)
	{
		res = GeometryAlgorithm::PointInPolygon(testPoints[i], polygon);

		if (res)
		{
			pointsInside.push_back(testPoints[i]);
		}
		else
		{
			pointsOutside.push_back(testPoints[i]);
		}

		printf("TestPointInPolygon result for point: %s -> %d\n", testPoints[i].toString().c_str(), res);
	}

	GLUTWrapper::InitWindow(&RenderDemoPointInPolygon);
	GLWrapper::InitRenderer();

	plm1 = new LineListMesh(polygon, Vec3(0, 1, 1), 6.0f);
	plm2 = new PointListMesh(pointsInside, Vec3(1, 1, 0), 3.0f);
	plm3 = new PointListMesh(pointsOutside, Vec3(1, 0, 0), 1.0f);

	GLUTWrapper::RenderLoop();
}