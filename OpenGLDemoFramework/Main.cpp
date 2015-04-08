#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Bar.hpp"
#include "Cube.hpp"
#include "Rectangle.hpp"
#include <cmath>
#include "TextureGenerator.hpp"
#define TEX_SIZE 32
Bar* b;
Cube* c;
Rectangle* r1;
Rectangle* r2;
TextureGenerator gen;
unsigned char texture[TEX_SIZE * TEX_SIZE * 4];

void RenderScene()
{
	GLWrapper::ClearWindow();

	//b->Render();
	//r1->Render();
	//r2->Render();
	//GLWrapper::RenderTexture(texture, TEX_SIZE, TEX_SIZE);
	c->Render();
	GLUTWrapper::UpdateFrame();
	GLUTWrapper::RequestNewFrame();
}


int main(int argc, char* argv[])
{
	GLUTWrapper::InitWindow(&RenderScene);
	GLWrapper::InitRenderer();
	
	//b = new Bar();
	//r1 = new Rectangle(/*Vec2(-0.5,0.5), Vec2(0.5,-0.5)*/);
	//r1 = new Rectangle(Vec2(-1,1), Vec2(1,-1));
	r1 = new Rectangle();

	//for (int i = 0; i < TEX_SIZE * TEX_SIZE * 4; i += 4)
	//{
	//	texture[i] = sin(i * 100) * 100;
	//	texture[i + 1] = 100;
	//	texture[i + 2] = sin(i * 100) * 100;
	//	texture[i + 3] = 255;
	//}

	for (int y = 0; y < TEX_SIZE; y += 1)
	{
		for (int x = 0; x < TEX_SIZE; x += 1)
		{
			const int row = y * TEX_SIZE * 4;
			const int offset = row + x * 4;

			texture[offset] = (y * 255) / TEX_SIZE;
			texture[offset + 1] = (x * 255) / TEX_SIZE;
			texture[offset + 2] = 127;
			texture[offset + 3] = 255;
		}
	}

	unsigned int* tex = gen.generateGradient();
	//r1->attachTexture(256, 1, tex);
	//r1->attachShaders("Shaders/tex.vs", "Shaders/mandelbulb.fs");
	//r2->attachTexture(TEX_SIZE, TEX_SIZE, texture);
	c = new Cube();
	GLUTWrapper::RenderLoop();

	return 0;
}
