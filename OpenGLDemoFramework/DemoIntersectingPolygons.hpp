#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "LineListMesh.hpp"

static LineListMesh* p1;
static LineListMesh* p2;
static LineListMesh* p3;

static void RenderScene()
{
	GLWrapper::ClearWindow();

	p1->Render();
	p2->Render();
	p3->Render();
	
	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

void DemoIntersectingPolygons()
{
	std::vector<Vec2> inputPolygon1;
	std::vector<Vec2> inputPolygon2;
	std::vector<Vec2> outputPolygon;

	inputPolygon1.push_back(Vec2(7, 4));
	inputPolygon1.push_back(Vec2(10, 7));
	inputPolygon1.push_back(Vec2(8, 10));
	inputPolygon1.push_back(Vec2(4, 11));
	inputPolygon1.push_back(Vec2(1, 5));
	inputPolygon1.push_back(Vec2(2, 2));

	inputPolygon2.push_back(Vec2(7, 4));
	inputPolygon2.push_back(Vec2(10, 7));
	inputPolygon2.push_back(Vec2(8, 10));
	inputPolygon2.push_back(Vec2(4, 11));
	inputPolygon2.push_back(Vec2(1, 5));
	inputPolygon2.push_back(Vec2(2, 2));

	outputPolygon = GeometryAlgorithm::IntersectPolygons(inputPolygon1, inputPolygon2);

	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	p1 = new LineListMesh(inputPolygon1, Vec3(1, 0, 0), 4.0f);
	p2 = new LineListMesh(inputPolygon2, Vec3(1, 1, 0), 4.0f);
	p3 = new LineListMesh(outputPolygon, Vec3(0, 1, 1), 4.0f);
	
	GLUTWrapper::RenderLoop();
}