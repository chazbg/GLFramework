#pragma once
#include "Texture.hpp"
#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Rectangle.hpp"
#include "TextureGenerator.hpp"
#include "Cube.hpp"
#include "BlockMesh.hpp"
#include "FrameBuffer.hpp"
#include "GeometryAlgorithm.hpp"
#include "Triangle.hpp"
#include <GL/glew.h>
namespace TexDemo
{
	BlockMesh* c;
	Cube* c1;
	Triangle* t;

	void RenderDemoTex()
	{
		GLWrapper::ClearWindow();
		//t->Render();
		c->Render();
		//c1->Render();
		GLUTWrapper::UpdateFrame();
		GLUTWrapper::RequestNewFrame();
	}

	void DemoTex()
	{
		GLUTWrapper::InitWindow(&RenderDemoTex);
		GLWrapper::InitRenderer();

		//t = new Triangle();
		c = new BlockMesh(1, 1, 1);
		//c = new BlockMesh(0.2, 0.4, 0.2);
		//c1 = new Cube();
		GLUTWrapper::RenderLoop();
	}

}