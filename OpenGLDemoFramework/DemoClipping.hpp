#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "LineListMesh.hpp"

namespace CLPDemo
{

LineListMesh* plm1;
LineListMesh* plm2;
LineListMesh* plm3;

static void RenderDemoClipping()
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
	std::vector<Vec2> inputPolygon;
	std::vector<Vec2> clippingPolygon;
	std::vector<Vec2> outputPolygon;
	inputPolygon.push_back(Vec2(4, 1));
	inputPolygon.push_back(Vec2(7, 3));
	inputPolygon.push_back(Vec2(9, 15));
	inputPolygon.push_back(Vec2(15, 5));
	inputPolygon.push_back(Vec2(10, 6));
	inputPolygon.push_back(Vec2(7, 5));

	clippingPolygon.push_back(Vec2(3, 3));
	clippingPolygon.push_back(Vec2(6, 1));
	clippingPolygon.push_back(Vec2(11, 1));
	clippingPolygon.push_back(Vec2(13.5, 3));
	clippingPolygon.push_back(Vec2(8.5, 8.5));

	outputPolygon = GeometryAlgorithm::Clip(inputPolygon, clippingPolygon);
	printf("Sutherland-Hodgeman clipping points:\n");
	for (unsigned int i = 0; i < outputPolygon.size(); i++)
	{
		printf("%s\n", outputPolygon[i].toString().c_str());
	}

	GLUTWrapper::InitWindow(&RenderDemoClipping);
	GLWrapper::InitRenderer();

	plm1 = new LineListMesh(inputPolygon, Vec3(0, 1, 1), 6.0f);
	plm2 = new LineListMesh(outputPolygon, Vec3(1, 1, 0), 3.0f);
	plm3 = new LineListMesh(clippingPolygon, Vec3(1, 0, 0), 1.0f);

	GLUTWrapper::RenderLoop();
}

}