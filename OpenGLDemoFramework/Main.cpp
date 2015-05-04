#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Rectangle.hpp"
#include "TextureGenerator.hpp"

Rectangle* r1;

void RenderScene()
{
	GLWrapper::ClearWindow();

	r1->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	TextureGenerator gen;

	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	r1 = new Rectangle(Vec2(-1,1), Vec2(1,-1));

	unsigned int* tex = gen.generateGradient();
	r1->attachTexture(256, 1, tex);
	r1->attachShaders("Shaders/tex.vs", "Shaders/mandelbulb.fs");

	GLUTWrapper::RenderLoop();

	return 0;
}
