#pragma once
#include "Texture.hpp"
#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Rectangle.hpp"
#include "TextureGenerator.hpp"
//#include "Cube.hpp"
#include "Cube.hpp"
#include "BlockMesh.hpp"
#include "FrameBuffer.hpp"
#include "GeometryAlgorithm.hpp"
#include "Triangle.hpp"
#include <GL/glew.h>
namespace TexDemo
{
	BlockMesh* c;
	BlockMesh* c1;
	BlockMesh* c2;
	BlockMesh* c3;
	BlockMesh* c4;
	Rectangle* r;
	FrameBuffer* fb;
	Triangle* t;
	unsigned int time;

	void RenderDemoTex()
	{
		GLWrapper::ClearWindow();

		c1->setTime(time);
		c2->setTime(time);
		c3->setTime(time);

		c1->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		c1->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 10, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c1->RenderToTexture(fb->getFbo(), r->GetTexId());

		glBindFramebuffer(GL_FRAMEBUFFER, fb->getFbo());

		c2->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		c2->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 10, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c2->Render();

		c3->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		c3->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 10, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c3->Render();

		c4->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		c4->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 10, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c4->Render();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		r->Render();

		c1->SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
		c1->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(-25, 15, 0), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c1->Render();

		c2->SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
		c2->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(-25, 15, 0), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c2->Render();

		c3->SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
		c3->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(-25, 15, 0), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c3->Render();

		c4->SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
		c4->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(-25, 15, 0), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c4->Render();

		time++;

		GLUTWrapper::UpdateFrame();
		GLUTWrapper::RequestNewFrame();
	}

	void DemoTex()
	{
		TextureGenerator gen;
		GLUTWrapper::InitWindow(&RenderDemoTex);
		GLWrapper::InitRenderer();
		time = 0;
		c1 = new BlockMesh(2.0f, 5.0f, 5.0f);
		c2 = new BlockMesh(2.0f, 5.0f, 5.0f);
		c2->SetPosition(Vec3(2, 0, -6));
		c3 = new BlockMesh(2.0f, 5.0f, 5.0f);
		c3->SetPosition(Vec3(-2, 0, 6));
		c4 = new BlockMesh(30.0f, 0.2f, 30.0f);
		c4->SetPosition(Vec3(0, -3, 0));

		r = new Rectangle(Vec2(0.5, 1), Vec2(1, 0.5));
		r->attachTexture(Texture(800, 800, 4, 0));

		fb = new FrameBuffer();

		GLUTWrapper::RenderLoop();
	}

}