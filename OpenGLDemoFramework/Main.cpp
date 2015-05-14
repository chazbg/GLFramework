#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "PlaneMesh.hpp"

PlaneMesh* p;

void RenderScene()
{
	GLWrapper::ClearWindow();

	p->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	p = new PlaneMesh(50, 50);

	GLUTWrapper::RenderLoop();

	return 0;
}
