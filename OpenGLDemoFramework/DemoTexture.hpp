#pragma once
#include "Texture.hpp"
#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Rectangle.hpp"
#include "TextureGenerator.hpp"
#include "Cube.hpp"
#include "GeometryAlgorithm.hpp"

namespace TexDemo
{
	Cube* c;
	Rectangle* r;

	void RenderDemoTex()
	{
		GLWrapper::ClearWindow();

		c->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		c->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(4, 4, -4), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c->RenderToTexture(r->GetTexId());
		r->Render();

		c->SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
		c->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 5, -5), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c->Render();

		GLUTWrapper::UpdateFrame();
		GLUTWrapper::RequestNewFrame();
	}

	void DemoTex()
	{
		TextureGenerator gen;
		GLUTWrapper::InitWindow(&RenderDemoTex);
		GLWrapper::InitRenderer();
		c = new Cube();
		//c->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		r = new Rectangle();
		//r->attachTexture(Texture(256, 1, 4, (unsigned char*) gen.generateGradient()));
		r->attachTexture(Texture(800, 800, 4, 0));
		GLUTWrapper::RenderLoop();
	}

}