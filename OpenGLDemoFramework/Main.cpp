#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Rectangle.hpp"
#include "TextureGenerator.hpp"
#include "Cube.hpp"

Cube* c;
Rectangle* r1;
Rectangle* r2;

void RenderScene()
{
	GLWrapper::ClearWindow();

	c->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	TextureGenerator gen;

	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	c = new Cube();

	GLUTWrapper::RenderLoop();

	return 0;
}
