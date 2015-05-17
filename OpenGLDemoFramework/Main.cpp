#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "TestGeometryAlgorithm.hpp"

void RenderScene()
{
	GLWrapper::ClearWindow();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	TestPointInPolygon();

	GLUTWrapper::RenderLoop();

	return 0;
}
