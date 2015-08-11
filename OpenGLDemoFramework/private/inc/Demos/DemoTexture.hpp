#pragma once
#include "Core/Texture.hpp"
#include "Windowing/GLUTWrapper.hpp"
#include "Rendering/GLWrapper.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/TextureGenerator.hpp"
#include "Geometry/Cube.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Core/FrameBuffer.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Geometry/Triangle.hpp"
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

	void RenderDemoTex()
	{
		GLWrapper::ClearWindow();

		c1->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		c1->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 0, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c1->RenderToTexture(fb->getFbo(), r->GetTexId());

		glBindFramebuffer(GL_FRAMEBUFFER, fb->getFbo());

		c2->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		c2->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 0, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c2->Render();

		c3->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		c3->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 0, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c3->Render();

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

		GLUTWrapper::UpdateFrame();
		GLUTWrapper::RequestNewFrame();
	}

	void DemoTex()
	{
		TextureGenerator gen;
		GLUTWrapper::InitWindow(&RenderDemoTex);
		GLWrapper::InitRenderer();

		c1 = new BlockMesh(2.0f, 5.0f, 5.0f);
		c2 = new BlockMesh(2.0f, 5.0f, 5.0f);
		c2->SetPosition(Vec3(2, 0, -6));
		c3 = new BlockMesh(2.0f, 5.0f, 5.0f);
		c3->SetPosition(Vec3(-2, 0, 6));
		
		r = new Rectangle(Vec2(0.5, 1), Vec2(1, 0.5));
		r->attachTexture(Texture(800, 800, 4, 0));

		fb = new FrameBuffer();

		GLUTWrapper::RenderLoop();
	}

}