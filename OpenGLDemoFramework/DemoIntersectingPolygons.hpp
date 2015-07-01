#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "LineListMesh.hpp"

static LineListMesh* p1;
static LineListMesh* p2;
static LineListMesh* p3;

static void RenderDemoIntersectingPolygons()
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

	inputPolygon1.push_back(Vec2(2, 2));
	inputPolygon1.push_back(Vec2(5, 2));
	inputPolygon1.push_back(Vec2(8, 6));
	inputPolygon1.push_back(Vec2(6, 8));
	inputPolygon1.push_back(Vec2(2, 9));
	inputPolygon1.push_back(Vec2(1, 6));

	inputPolygon2.push_back(Vec2(0, 4.5));
	inputPolygon2.push_back(Vec2(3, 3));
	inputPolygon2.push_back(Vec2(7, 3));
	inputPolygon2.push_back(Vec2(9, 4));
	inputPolygon2.push_back(Vec2(10, 8));
	inputPolygon2.push_back(Vec2(4, 7));

	outputPolygon = GeometryAlgorithm::IntersectPolygons(inputPolygon1, inputPolygon2);

	GLUTWrapper::InitWindow(&RenderDemoIntersectingPolygons);
	GLWrapper::InitRenderer();

	p1 = new LineListMesh(inputPolygon1, Vec3(1, 0, 0), 4.0f);
	p2 = new LineListMesh(inputPolygon2, Vec3(1, 1, 0), 4.0f);
	p3 = new LineListMesh(outputPolygon, Vec3(0, 1, 1), 4.0f);
	
	GLUTWrapper::RenderLoop();
}