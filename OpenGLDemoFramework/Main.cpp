#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Bar.hpp"
#include "Triangle.hpp"

Bar* b;
Triangle* t;

void RenderScene()
{
	GLWrapper::ClearWindow();

	t->Render();
	//b->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();
	
	//b = new Bar();
	t = new Triangle();

	GLUTWrapper::RenderLoop();
	return 0;
}
