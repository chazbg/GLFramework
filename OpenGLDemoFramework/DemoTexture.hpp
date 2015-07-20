#pragma once
#include "Texture.hpp"
#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Rectangle.hpp"
#include "TextureGenerator.hpp"
#include "Cube.hpp"

namespace TexDemo
{
	Cube* c;
	Rectangle* r;

	void RenderDemoTex()
	{
		GLWrapper::ClearWindow();

		c->RenderToTexture(r->GetTexId());
		r->Render();

		GLUTWrapper::UpdateFrame();
		GLUTWrapper::RequestNewFrame();
	}

	void DemoTex()
	{
		TextureGenerator gen;
		GLUTWrapper::InitWindow(&RenderDemoTex);
		GLWrapper::InitRenderer();
		c = new Cube();
		r = new Rectangle();
		//r->attachTexture(Texture(256, 1, 4, (unsigned char*) gen.generateGradient()));
		r->attachTexture(Texture(800, 800, 4, 0));
		GLUTWrapper::RenderLoop();
	}

}