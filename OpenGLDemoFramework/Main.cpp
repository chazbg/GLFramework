#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Triangle.hpp"
#include "PlaneMesh.hpp"

Triangle* t;
PlaneMesh* p;

void RenderScene()
{
	GLWrapper::ClearWindow();

	p->Render();
	//t->Render();
	
	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	p = new PlaneMesh(50,50);
	//t = new Triangle();

	GLUTWrapper::RenderLoop();

	return 0;
}
