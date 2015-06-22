#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "TestGeometryAlgorithm.hpp"
#include "PointListMesh.hpp"

PointListMesh* plm1;
PointListMesh* plm2;

void RenderScene()
{
	GLWrapper::ClearWindow();

	plm1->Render();
	plm2->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	std::vector<Vec2> clippedPolygon, clippingPolygon;
	TestClip(clippedPolygon, clippingPolygon);

	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	plm1 = new PointListMesh(clippedPolygon);
	plm2 = new PointListMesh(clippingPolygon, Vec3(1, 1, 0), 3.0f);

	GLUTWrapper::RenderLoop();

	return 0;
}
