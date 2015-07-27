#pragma once
#include "Texture.hpp"
#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Rectangle.hpp"
#include "TextureGenerator.hpp"
//#include "Cube.hpp"
#include "BlockMesh.hpp"
#include "FrameBuffer.hpp"
#include "GeometryAlgorithm.hpp"
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

	void RenderDemoTex()
	{
		GLWrapper::ClearWindow();

		c1->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		//c1->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0.3, 0.3, 0.3), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c1->RenderToTexture(fb->getFbo(), r->GetTexId());

		glBindFramebuffer(GL_FRAMEBUFFER, fb->getFbo());

		c2->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		//c2->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0.3, 0.3, 0.3), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c2->Render();

		glBindFramebuffer(GL_FRAMEBUFFER, fb->getFbo());

		c3->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		//c3->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0.3, 0.3, 0.3), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c3->Render();

		c4->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		c4->Render();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE, r->GetTexId());
		//r->Render();

		c1->SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
		//c1->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 1, -1), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c1->Render();

		c2->SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
		//c2->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 5, -5), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c2->Render();

		c3->SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
		//c3->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 5, -5), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c3->Render();

		c4->SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
		//c3->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 5, -5), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c4->Render();

		//c->Render();

		//c1->Render();
		//c2->Render();
		//c3->Render();

		GLUTWrapper::UpdateFrame();
		GLUTWrapper::RequestNewFrame();
	}

	void DemoTex()
	{
		TextureGenerator gen;
		GLUTWrapper::InitWindow(&RenderDemoTex);
		GLWrapper::InitRenderer();
		//c = new Cube();
		//c = new BlockMesh(1, 1, 1);
		c1 = new BlockMesh(0.2f, 0.5, 0.5);
		c1->SetPosition(Vec3(-0.5, 0, -0.3));
		c2 = new BlockMesh(0.2f, 0.5, 0.5);
		c2->SetPosition(Vec3(0, 0, 0));
		c3 = new BlockMesh(0.2f, 0.5, 0.5);
		c3->SetPosition(Vec3(0.5, 0, 0.3));
		c4 = new BlockMesh(2, 0.01f, 2);
		c4->SetPosition(Vec3(0, -0.1, 0));
		//c->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		r = new Rectangle();
		//r->attachTexture(Texture(256, 1, 4, (unsigned char*) gen.generateGradient()));
		r->attachTexture(Texture(800, 800, 4, 0));

		fb = new FrameBuffer();
		GLUTWrapper::RenderLoop();
	}

}