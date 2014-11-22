#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Bar.hpp"

#define TEX_SIZE 512
Bar* b;
unsigned char texture[TEX_SIZE * TEX_SIZE];
void RenderScene()
{
	GLWrapper::ClearWindow();

	//b->Render();
	GLWrapper::RenderTexture(texture, TEX_SIZE, TEX_SIZE);
	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}

int main(int argc, char* argv[])
{
	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();
	
	b = new Bar();

	for (int i = 0; i < TEX_SIZE; i++)
	{
		for (int j = 0; j < TEX_SIZE; j++)
		{
			texture[i * TEX_SIZE + j] = j > 255 ? 255 : j;
		}
	}

	GLUTWrapper::RenderLoop();
	return 0;
}
