#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Bar.hpp"

Bar* b;

void RenderScene()
{
	GLWrapper::ClearWindow();

	b->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();
	
	b = new Bar();

	GLUTWrapper::RenderLoop();
	return 0;
}
