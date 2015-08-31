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
#include <vector>
#include "TextRenderer.hpp"

using namespace std;

namespace TexDemo
{
	BlockMesh* c;
	BlockMesh* c1;
	vector<BlockMesh*> meshes;
	Rectangle* r;
	FrameBuffer* fb;
	Triangle* t;
	unsigned int time;
	bool stopTime;
	Vec3 cameraPos;
	TextRenderer* textRenderer;

	void KeyboardCallback(unsigned char c, int x, int y)
	{
		cout << c << " " << x << " " << y << endl;
		stopTime = !stopTime;
		
			switch (c)
			{

			case 'j':
				time -= 5;
				break;
			case 'l':
				time += 5;
				break;
			case 'a':
				cameraPos.x -= 1;
				break;
			case 's':
				cameraPos.y -= 1;
				break;
			case 'd':
				cameraPos.x += 1;
				break;
			case 'w':
				cameraPos.y += 1;
				break;
			case 'q':
				cameraPos.z -= 1;
				break;
			case 'e':
				cameraPos.z += 1;
				break;
			default:
				stopTime = !stopTime;
				break;
			}
	}

	void MouseCallback(int button, int state, int x, int y)
	{
		cout << button << " " << state << " " << x << " " << y << endl;
		stopTime = !stopTime;
	}

	void RenderDemoTex()
	{
		GLWrapper::ClearWindow();

		c1->setTime(time);
		meshes[0]->setTime(time);
		meshes[1]->setTime(time);

		c1->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
		c1->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 10, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c1->RenderToTexture(fb->getFbo(), r->GetTexId());

		glBindFramebuffer(GL_FRAMEBUFFER, fb->getFbo());

		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i]->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
			meshes[i]->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 10, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)));
			meshes[i]->Render();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		r->Render();

		c1->SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
		c1->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(cameraPos, Vec3(0, 0, 0), Vec3(0, 1, 0)));
		c1->Render();


		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i]->SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
			meshes[i]->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(cameraPos, Vec3(0, 0, 0), Vec3(0, 1, 0)));
			meshes[i]->Render();
		}

		time++;

		textRenderer->render("TEST", -1, 1);
		GLUTWrapper::UpdateFrame();
		GLUTWrapper::RequestNewFrame();
	}

	void DemoTex()
	{
		TextureGenerator gen;
		GLUTWrapper::InitWindow(&RenderDemoTex);
		GLUTWrapper::SetKeyboardCallback(KeyboardCallback);
		GLUTWrapper::SetMouseCallback(MouseCallback);
		GLWrapper::InitRenderer();
		
		time = 0;
		stopTime = false;
		cameraPos = Vec3(-25, 10, 0);
		c1 = new BlockMesh(2.0f, 5.0f, 5.0f);
		c1->SetPosition(Vec3(0, -3, 0));
		meshes.push_back(new BlockMesh(2.0f, 5.0f, 5.0f));
		meshes[0]->SetPosition(Vec3(-2, -3, -6));
		meshes.push_back(new BlockMesh(2.0f, 5.0f, 5.0f));
		meshes[1]->SetPosition(Vec3(2, -3, 6));
		meshes.push_back(new BlockMesh(20.0f, 0.2f, 20.0f));
		meshes[2]->SetPosition(Vec3(0, -3, 0));
		meshes.push_back(new BlockMesh(20.0f, 20.0f, 0.2f));
		meshes[3]->SetPosition(Vec3(0, -3, -10));

		r = new Rectangle(Vec2(0.5, 1), Vec2(1, 0.5));
		r->attachTexture(Texture(800, 800, 4, 0));

		fb = new FrameBuffer();

		textRenderer = new TextRenderer();
		textRenderer->init();

		GLUTWrapper::RenderLoop();
	}

}