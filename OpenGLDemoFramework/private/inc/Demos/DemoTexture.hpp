#pragma once
#include "Core/Texture.hpp"
#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/TextureGenerator.hpp"
#include "Geometry/Cube.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Core/FrameBuffer.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Geometry/Triangle.hpp"
#include <GL/glew.h>
#include <vector>
#include <Core/Text/TextRenderer.hpp>

using namespace std;

namespace TexDemo
{
	class TestWindowApp : public IApplication
	{
	public:
		TestWindowApp() : renderer(0) {}
		~TestWindowApp() {}
		virtual void onInit()
		{
			renderer = new Renderer();

			TextureGenerator gen;
			time = 0;
			stopTime = false;
			cameraPos = Vec3(0, 15, 25);
			meshPos = Vec3(-10.0, 3, 0);
			prevDir = Vec3(0.01f, 0.1f, 0);
			c1 = new BlockMesh(2.0f, 5.0f, 5.0f);
			c1->SetPosition(Vec3(0, -1, 0));
			meshes.push_back(new BlockMesh(2.0f, 5.0f, 5.0f));
			meshes[0]->SetPosition(Vec3(-2, -1, -6));
			meshes.push_back(new BlockMesh(2.0f, 5.0f, 5.0f));
			meshes[1]->SetPosition(Vec3(2, -1, 6));
			meshes.push_back(new BlockMesh(20.0f, 0.2f, 20.0f));
			meshes[2]->SetPosition(Vec3(0, -3, 0));
			meshes.push_back(new BlockMesh(20.0f, 20.0f, 0.2f));
			meshes[3]->SetPosition(Vec3(0, -3, -10));
			meshes.push_back(new BlockMesh(0.5f, 0.5f, 0.5f));
			//meshes[4]->SetPosition(Vec3(0, 10, 0));

			r = new Rectangle(Vec2(0.5, 1), Vec2(1, 0.5));
			r->attachTexture(Texture(800, 800, 4, 0));

			fb = new FrameBuffer();

			textRenderer = new TextRenderer();
			textRenderer->init();

			prevMousePos = Vec2(0.5, 0.5);
		}
		virtual void onUpdate(const unsigned int deltaTime) {}
		virtual void onRender(const unsigned int deltaTime)
		{
			renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

			c1->setTime(time);
			meshes[0]->setTime(time);
			meshes[1]->setTime(time);

			c1->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
			c1->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 15, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)));
			c1->RenderToTexture(fb->getFbo(), r->GetTexId());

			glBindFramebuffer(GL_FRAMEBUFFER, fb->getFbo());

			for (int i = 0; i < meshes.size(); i++)
			{
				meshes[i]->SetShaders("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
				meshes[i]->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 15, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)));
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

			prevDir += Vec3(0, -0.001f, 0);
			meshPos += prevDir;
			meshes[4]->SetPosition(meshPos);
			meshes[4]->SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
			meshes[4]->SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(cameraPos, Vec3(0, 0, 0), Vec3(0, 1, 0)));
			meshes[4]->Render();

			if (!stopTime)
			{
				time++;
			}

			textRenderer->render("TEST", -1, 1);
		}

		virtual void onDestroy()
		{
			delete renderer;
			delete c1;
			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				delete meshes[i];
			}

			delete r;
			delete fb;
		}
		virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
		virtual void onMouseEvent(int button, int state, int x, int y) 
		{ 
			cout << button << " " << state << " " << x << " " << y << endl;
			stopTime = !stopTime;
		}

		virtual void onKeyboardEvent(unsigned char c, int x, int y) 
		{ 
			cout << c << " " << x << " " << y << endl; 
			stopTime = !stopTime;
			Vec3 zAxis = (-cameraPos).normalize();
			Vec3 up(0, 1, 0);
			Vec3 xAxis = (zAxis * up).normalize();
			Vec3 yAxis = xAxis * zAxis;

			switch (c)
			{

			case 'j':
				time -= 5;
				break;
			case 'l':
				time += 5;
				break;
			case 'a':
				cameraPos -= xAxis;
				break;
			case 's':
				cameraPos -= yAxis;
				break;
			case 'd':
				cameraPos += xAxis;
				break;
			case 'w':
				cameraPos += yAxis;
				break;
			case 'q':
				cameraPos -= zAxis;
				break;
			case 'e':
				cameraPos += zAxis;
				break;
			case 'r':
				meshPos = Vec3(-10.0, 3, 0);
				prevDir = Vec3(0.01f, 0.1f, 0);
				break;
			default:
				stopTime = !stopTime;
				break;
			}
		}
		virtual void onMouseMove(int x, int y)
		{
			float nx = x / 800.0f;
			float ny = y / 800.0f;
			Vec2 delta = Vec2(nx, ny) - prevMousePos;

			Vec3 zAxis = (-cameraPos).normalize();
			Vec3 up(0, 1, 0);
			Vec3 xAxis = (zAxis * up).normalize();
			Vec3 yAxis = xAxis * zAxis;

			cameraPos -= xAxis * delta.x * 50;
			cameraPos += yAxis * delta.y * 50;

			prevMousePos = Vec2(nx, ny);
		}
	private:
		Renderer* renderer;
		BlockMesh* c1;
		vector<BlockMesh*> meshes;
		Rectangle* r;
		FrameBuffer* fb;
		unsigned int time;
		bool stopTime;
		Vec3 cameraPos;
		Vec3 meshPos;
		Vec3 prevDir;
		TextRenderer* textRenderer;
		Vec2 prevMousePos;
	};

	void main()
	{
		WindowParameters params;
		params.width = 800;
		params.height = 800;
		params.posX = 100;
		params.posY = 100;
		params.name = "TestWindow";
		TestWindowApp app;
		Window window(params, app);
		window.startRenderLoop();
	}
}