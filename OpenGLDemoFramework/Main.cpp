#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Bar.hpp"
#include "Rectangle.hpp"

#define TEX_SIZE 32
Bar* b;
Rectangle* r;

unsigned char texture[TEX_SIZE * TEX_SIZE * 4];

void RenderScene()
{
	GLWrapper::ClearWindow();

	//b->Render();
	r->Render();
	//GLWrapper::RenderTexture(texture, TEX_SIZE, TEX_SIZE);

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

#include "TestVector.hpp"

int main(int argc, char* argv[])
{
	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();
	
	//b = new Bar();
	r = new Rectangle(/*Vec2(-0.5,0.5), Vec2(0.5,-0.5)*/);
	for (int i = 0; i < TEX_SIZE * TEX_SIZE * 4; i += 4)
	{
		texture[i] = 0;
		texture[i + 1] = 100;
		texture[i + 2] = 0;
		texture[i + 3] = 255;
	}

	GLUTWrapper::RenderLoop();

	return 0;
}
