#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "PlaneMesh.hpp"
#include "Cube.hpp"
#include "Triangle.hpp"
PlaneMesh* p;
Cube* c;
Triangle* t;
void RenderScene()
{
	GLWrapper::ClearWindow();

	p->Render();
	c->Render();
	t->Render();
	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	p = new PlaneMesh(50, 50);
	c = new Cube();
	t = new Triangle();
	GLUTWrapper::RenderLoop();

	return 0;
}
