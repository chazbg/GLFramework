#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "TestGeometryAlgorithm.hpp"
#include "LineListMesh.hpp"

static LineListMesh* plm1;
static LineListMesh* plm2;
static LineListMesh* plm3;

static void RenderScene()
{
	GLWrapper::ClearWindow();

	plm1->Render();
	plm2->Render();
	plm3->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

void DemoClipping()
{
	std::vector<Vec2> inputPolygon, clippedPolygon, clippingPolygon;
	TestClip(inputPolygon, clippedPolygon, clippingPolygon);

	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	plm1 = new LineListMesh(inputPolygon, Vec3(0, 1, 1), 6.0f);
	plm2 = new LineListMesh(clippedPolygon, Vec3(1, 1, 0), 3.0f);
	plm3 = new LineListMesh(clippingPolygon, Vec3(1, 0, 0), 1.0f);

	GLUTWrapper::RenderLoop();
}