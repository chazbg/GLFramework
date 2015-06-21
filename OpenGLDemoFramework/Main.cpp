#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "GeometryAlgorithm.hpp"
#include "TestGeometryAlgorithm.hpp"

void RenderScene()
{
	GLWrapper::ClearWindow();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	TestClip();
	//GLUTWrapper::InitWindow(&RenderScene);
	//GLWrapper::InitRenderer();

	//GLUTWrapper::RenderLoop();

	return 0;
}
