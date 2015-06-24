#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "LineListMesh.hpp"
#include "PointListMesh.hpp"

static LineListMesh* llm;
static PointListMesh* plm;

static void RenderScene()
{
	GLWrapper::ClearWindow();

	llm->Render();
	plm->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

void DemoRotatingCalipers()
{
	std::vector<Vec2> inputPolygon;
	std::vector<Vec2> antipodPoints;
	inputPolygon.push_back(Vec2(7, 4));
	inputPolygon.push_back(Vec2(10, 7));
	inputPolygon.push_back(Vec2(8, 10));
	inputPolygon.push_back(Vec2(4, 11));
	inputPolygon.push_back(Vec2(1, 5));
	inputPolygon.push_back(Vec2(2, 2));

	antipodPoints = GeometryAlgorithm::RotatingCalipers(inputPolygon);

	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	llm = new LineListMesh(inputPolygon, Vec3(0, 1, 1), 5.0f);
	plm = new PointListMesh(antipodPoints, Vec3(1, 1, 0), 3.0f);

	GLUTWrapper::RenderLoop();
}