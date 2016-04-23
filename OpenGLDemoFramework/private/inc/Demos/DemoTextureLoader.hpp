#pragma once
#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include <vector>
#include <Core/PerspectiveCamera.hpp>
#include <Core/Scene.hpp>
#include <Core/ShaderMaterial.hpp>
#include <Core/TextureLoader.hpp>
#include <Geometry/BlockMesh.hpp>

using namespace std;

namespace TextureLoaderDemo
{
	class TestWindowApp : public IApplication
	{
	public:
		TestWindowApp() : renderer(0) {}
		~TestWindowApp() {}
		virtual void onInit()
		{
			TextureLoader texLoader;

			renderer = new Renderer(Vec2(1024, 1024));

			time = 0;
			stopTime = false;
			cameraPos = Vec3(0, 15, 25);
			cubeTexturedMat = new OpenGLMaterial("Shaders/texturedCube.vs", "Shaders/texturedCube.fs");
			cubeTexturedMat->addTexture(texLoader.loadTexture("Images/DefaultTexture.png"));
			cubeTexturedMat->setProperty("colorMap", 0);
			cubeTexturedMat->setProperty("sampler", 1);
			cubeTexturedMat->setProperty("diffuse", Vec3(0, 0, 0.8f));

			meshes.push_back(new BlockMesh(20.0f, 20.0f, 0.2f));
			meshes[0]->SetPosition(Vec3(0, -3, -10));
			meshes[0]->setMaterial(cubeTexturedMat);

			scene.add(meshes[0]);

			prevMousePos = Vec2(0.5, 0.5);
		}
		virtual void onUpdate(const unsigned int deltaTime) {}
		virtual void onRender(const unsigned int deltaTime)
		{
			renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

			camera.setPosition(cameraPos);

			meshes[0]->SetRotation(0, time * 0.01f, 0);
			cubeTexturedMat->setProperty("time", (float)time);

			renderer->render(scene, camera);
			if (!stopTime)
			{
				time++;
			}
		}

		virtual void onDestroy()
		{
			delete renderer;

			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				delete meshes[i];
			}
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
			default:
				stopTime = !stopTime;
				break;
			}
		}

		virtual void onMouseMove(int x, int y)
		{
			float nx = x / 1024.0f;
			float ny = y / 1024.0f;
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
		vector<BlockMesh*> meshes;
		unsigned int time;
		bool stopTime;
		Vec3 cameraPos;
		Scene scene;
		PerspectiveCamera camera;
		OpenGLMaterial* cubeTexturedMat;
		Vec2 prevMousePos;
	};

	void main()
	{
		WindowParameters params;
		params.width = 1024;
		params.height = 1024;
		params.posX = 100;
		params.posY = 100;
		params.name = "TestWindow";
		TestWindowApp app;
		Window window(params, app);
		window.startRenderLoop();
	}
}