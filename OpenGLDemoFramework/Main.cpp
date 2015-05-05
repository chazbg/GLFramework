#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Rectangle.hpp"
#include "TextureGenerator.hpp"

Rectangle* r1;
Rectangle* r2;
void RenderScene()
{
	GLWrapper::ClearWindow();

	r1->Render();
	r2->Render();

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	TextureGenerator gen;

	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();

	r1 = new Rectangle(Vec2(-1, 1), Vec2(0, -1));
	r2 = new Rectangle(Vec2(0, 1), Vec2(1, -1));
	unsigned int* tex = gen.generateGradient();
	r1->attachTexture(256, 1, tex);
	r1->attachShaders("Shaders/tex.vs", "Shaders/fractal.fs");
	r2->attachTexture(256, 1, tex);
	r2->attachShaders("Shaders/tex.vs", "Shaders/mandelbulb.fs");

	GLUTWrapper::RenderLoop();

	return 0;
}
