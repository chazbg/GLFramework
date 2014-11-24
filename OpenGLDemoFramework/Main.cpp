#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Bar.hpp"

#define TEX_SIZE 32
Bar* b;
unsigned char texture[TEX_SIZE * TEX_SIZE * 4];

void RenderScene()
{
	GLWrapper::ClearWindow();

	b->Render();

	//GLWrapper::RenderTexture(texture, TEX_SIZE, TEX_SIZE);

	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();
	
	b = new Bar();

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
