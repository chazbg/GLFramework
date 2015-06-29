#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "TestGeometryAlgorithm.hpp"
#include "LineListMesh.hpp"
#include "LineSegment.hpp"
#include "PointListMesh.hpp"
#include <iostream>

static std::vector<LineListMesh*> lm1;
static PointListMesh* plm;

static void RenderScene()
{
	GLWrapper::ClearWindow();

	for (unsigned int i = 0; i < lm1.size(); i++)
	{
		lm1[i]->Render();
	}

	plm->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

std::vector<std::vector<LineSegment3>> initCubeSides(const std::vector<Vec3>& p)
{
	std::vector<std::vector<LineSegment3>> sides;
	for (unsigned int i = 0; i < 6; i++)
	{
		sides.push_back(std::vector<LineSegment3>());
	}

	sides[0].push_back(LineSegment3(p[0], p[1]));
	sides[0].push_back(LineSegment3(p[1], p[2]));
	sides[0].push_back(LineSegment3(p[2], p[3]));
	sides[0].push_back(LineSegment3(p[3], p[0]));

	sides[1].push_back(LineSegment3(p[0], p[1]));
	sides[1].push_back(LineSegment3(p[1], p[5]));
	sides[1].push_back(LineSegment3(p[5], p[4]));
	sides[1].push_back(LineSegment3(p[4], p[0]));

	sides[2].push_back(LineSegment3(p[0], p[3]));
	sides[2].push_back(LineSegment3(p[3], p[7]));
	sides[2].push_back(LineSegment3(p[7], p[4]));
	sides[2].push_back(LineSegment3(p[4], p[0]));

	sides[3].push_back(LineSegment3(p[2], p[1]));
	sides[3].push_back(LineSegment3(p[1], p[5]));
	sides[3].push_back(LineSegment3(p[5], p[6]));
	sides[3].push_back(LineSegment3(p[6], p[2]));

	sides[4].push_back(LineSegment3(p[2], p[3]));
	sides[4].push_back(LineSegment3(p[3], p[7]));
	sides[4].push_back(LineSegment3(p[7], p[6]));
	sides[4].push_back(LineSegment3(p[6], p[2]));

	sides[5].push_back(LineSegment3(p[4], p[5]));
	sides[5].push_back(LineSegment3(p[5], p[6]));
	sides[5].push_back(LineSegment3(p[6], p[7]));
	sides[5].push_back(LineSegment3(p[7], p[4]));

	return sides;
}

void DemoProjections()
{
	std::vector<std::vector<Vec3>*> inputPolygon;
	std::vector<Vec3> points;
	
	Vec3 A(0, 0, 0);
	Vec3 B(2, 0, 0);
	Vec3 C(2, 0, 2);
	Vec3 D(0, 0, 2);
	Vec3 E(0, 2, 0);
	Vec3 F(2, 2, 0);
	Vec3 G(2, 2, 2);
	Vec3 H(0, 2, 2);

	points.push_back(A);
	points.push_back(B);
	points.push_back(C);
	points.push_back(D);
	points.push_back(E);
	points.push_back(F);
	points.push_back(G);
	points.push_back(H);

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

	std::vector<std::vector<LineSegment3>> sides = initCubeSides(points);
	std::vector<Vec3> outPoints;
	for (unsigned int i = 0; i < 6; i++)
	{
		for (unsigned int j = 0; j < 6; j++)
		{
			if (i == j)	continue;

			for (unsigned int k = 0; k < 4; k++)
			{
				for (unsigned int l = 0; l < 4; l++)
				{
					if (sides[i][k] == sides[j][l]) continue;

					Vec3 fi, fj;
					if (GeometryAlgorithm::ComputeIntersection3(sides[i][k].a, sides[i][k].b, sides[j][l].a, sides[j][l].b, fi, fj))
					{
						if (outPoints.end() == std::find(outPoints.begin(), outPoints.end(), fi))
						{
							outPoints.push_back(fi);
						}

						if (outPoints.end() == std::find(outPoints.begin(), outPoints.end(), fj))
						{
							outPoints.push_back(fj);
						}

						if (fi.z > fj.z)
						{
							for (int m = 0; m < 4; m++)
							{
								sides[i][m] = LineSegment3(Vec3(0, 0, 0), Vec3(0, 0, 0));
							}
						}
						else
						{
							for (int m = 0; m < 4; m++)
							{
								sides[j][m] = LineSegment3(Vec3(0, 0, 0), Vec3(0, 0, 0));
							}
						}
					}
				}
			}
		}
	}
	//Vec3 fi, fj;
	//std::cout << GeometryAlgorithm::ComputeIntersection3(A, E, H, G, fi, fj) << std::endl;
	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	for (int i = 0; i < sides.size(); i++)
	{
		std::vector<Vec3> lines;
		lines.push_back(sides[i][0].a);
		lines.push_back(sides[i][0].b);
		lines.push_back(sides[i][1].a);
		lines.push_back(sides[i][1].b);
		lines.push_back(sides[i][2].a);
		lines.push_back(sides[i][2].b);
		lines.push_back(sides[i][3].a);
		lines.push_back(sides[i][3].b);
		lm1.push_back(new LineListMesh(lines, Vec3(0, 1, 1), 2.0f));
	}
	/*for (unsigned int i = 0; i < inputPolygon.size(); i++)
	{
		lm1.push_back(new LineListMesh(*inputPolygon[i], Vec3(0, 1, 1), 2.0f));
	}*/

	plm = new PointListMesh(outPoints, Vec3(1, 1, 0), 5.0f);
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