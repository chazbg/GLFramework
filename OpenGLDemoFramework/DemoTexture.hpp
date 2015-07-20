#pragma once
#include "Texture.hpp"
#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Rectangle.hpp"
#include "TextureGenerator.hpp"

namespace TexDemo
{
	Rectangle* r;

	void RenderDemoTex()
	{
		GLWrapper::ClearWindow();

		r->Render();

		GLUTWrapper::UpdateFrame();
		GLUTWrapper::RequestNewFrame();
	}

	void DemoTex()
	{
		TextureGenerator gen;
		GLUTWrapper::InitWindow(&RenderDemoTex);
		GLWrapper::InitRenderer();
		r = new Rectangle();
		r->attachTexture(Texture(256, 1, 4, (unsigned char*) gen.generateGradient()));
		GLUTWrapper::RenderLoop();
	}

}