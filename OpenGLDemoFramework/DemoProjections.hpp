#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "TestGeometryAlgorithm.hpp"
#include "LineListMesh.hpp"

static std::vector<LineListMesh*> lm1;

static void RenderScene()
{
	GLWrapper::ClearWindow();

	for (unsigned int i = 0; i < lm1.size(); i++)
	{
		lm1[i]->Render();
	}

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

void DemoProjections()
{
	std::vector<std::vector<Vec3>*> inputPolygon;
	Vec3 A(0, 0, 0);
	Vec3 B(2, 0, 0);
	Vec3 C(2, 0, 2);
	Vec3 D(0, 0, 2);
	Vec3 E(0, 2, 0);
	Vec3 F(2, 2, 0);
	Vec3 G(2, 2, 2);
	Vec3 H(0, 2, 2);

	for (unsigned int i = 0; i < 12; i++)
	{
		inputPolygon.push_back(new std::vector<Vec3>());
	}

	inputPolygon[0]->push_back(A);
	inputPolygon[0]->push_back(B);

	inputPolygon[1]->push_back(A);
	inputPolygon[1]->push_back(D);

	inputPolygon[2]->push_back(A);
	inputPolygon[2]->push_back(E);

	inputPolygon[3]->push_back(C);
	inputPolygon[3]->push_back(B);

	inputPolygon[4]->push_back(C);
	inputPolygon[4]->push_back(D);

	inputPolygon[5]->push_back(C);
	inputPolygon[5]->push_back(G);

	inputPolygon[6]->push_back(F);
	inputPolygon[6]->push_back(E);

	inputPolygon[7]->push_back(F);
	inputPolygon[7]->push_back(G);

	inputPolygon[8]->push_back(F);
	inputPolygon[8]->push_back(B);

	inputPolygon[9]->push_back(H);
	inputPolygon[9]->push_back(D);

	inputPolygon[10]->push_back(H);
	inputPolygon[10]->push_back(E);

	inputPolygon[11]->push_back(H);
	inputPolygon[11]->push_back(G);

	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	for (unsigned int i = 0; i < inputPolygon.size(); i++)
	{
		lm1.push_back(new LineListMesh(*inputPolygon[i], Vec3(0, 1, 1), 2.0f));
	}

	GLUTWrapper::RenderLoop();
}

void DemoProjections2()
{
	std::vector<std::vector<Vec3>*> inputPolygon;
	Vec3 A(0, 0, 0);
	Vec3 B(2, 0, 0);
	Vec3 C(2, 0, 2);
	Vec3 D(0, 0, 2);
	Vec3 E(1, 2.4, 1);
	Vec3 F(1, -2.4, 1);

	for (unsigned int i = 0; i < 12; i++)
	{
		inputPolygon.push_back(new std::vector<Vec3>());
	}

	inputPolygon[0]->push_back(A);
	inputPolygon[0]->push_back(B);

	inputPolygon[1]->push_back(A);
	inputPolygon[1]->push_back(D);

	inputPolygon[2]->push_back(C);
	inputPolygon[2]->push_back(B);

	inputPolygon[3]->push_back(C);
	inputPolygon[3]->push_back(D);

	inputPolygon[4]->push_back(A);
	inputPolygon[4]->push_back(E);

	inputPolygon[5]->push_back(A);
	inputPolygon[5]->push_back(F);

	inputPolygon[6]->push_back(B);
	inputPolygon[6]->push_back(E);

	inputPolygon[7]->push_back(B);
	inputPolygon[7]->push_back(F);

	inputPolygon[8]->push_back(C);
	inputPolygon[8]->push_back(E);

	inputPolygon[9]->push_back(C);
	inputPolygon[9]->push_back(F);

	inputPolygon[10]->push_back(D);
	inputPolygon[10]->push_back(E);

	inputPolygon[11]->push_back(D);
	inputPolygon[11]->push_back(F);

	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	for (unsigned int i = 0; i < inputPolygon.size(); i++)
	{
		lm1.push_back(new LineListMesh(*inputPolygon[i], Vec3(0, 1, 1), 2.0f));
	}

	GLUTWrapper::RenderLoop();
}