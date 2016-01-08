#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include <Core/PerspectiveCamera.hpp>
#include <Core/Scene.hpp>
#include <Core/ShaderMaterial.hpp>
#include <Core/TextureLoader.hpp>
#include <Geometry/CustomGeometry.hpp>
#include <Math/GeometryAlgorithm.hpp>
#include <iostream>
using namespace std;

namespace PBRDemo
{
	class TestWindowApp : public IApplication
	{
	public:
		TestWindowApp() : renderer(0) {}
		~TestWindowApp() {}
		virtual void onInit()
		{
			TextureLoader texLoader;

			renderer = new Renderer(Vec2(800, 800));

			shaderMaterial = new ShaderMaterial("Shaders/texturedCube.vs", "Shaders/texturedCube.fs");
			shaderMaterial->addTexture(texLoader.loadTexture("Images/Football.png"));
			shaderMaterial->addTextureCubemap(texLoader.loadTextureCubemap(
				"Images/front.jpg", 
				"Images/back.jpg", 
				"Images/top.jpg", 
				"Images/bottom.jpg", 
				"Images/left.jpg", 
				"Images/right.jpg"));

			shaderMaterial->setProperty("colorMap", 0);
			shaderMaterial->setProperty("sampler", 1);
			shaderMaterial->setProperty("cubeMap", 2);
            
			g = new CustomGeometry("3DAssets/female_elf-3ds.3DS");
			g->setMaterial(shaderMaterial);
            g->Scale(0.1f, 0.1f, 0.1f);
            g->Rotate(-3.14f / 2.0f, 0, 0);

           // environmentCube = new CustomGeometry("3DAssets/Toy_Cube.3ds");
           // environmentCube->setMaterial(shaderMaterial);
           // environmentCube->setModelMatrix(GeometryAlgorithm::CreateSRTMatrix(Vec3(10,10,10), Vec3(0, 0, 0), Vec3(0, 0, 0)));


			scene.add(g);
            //scene.add(environmentCube);
			time = 0;
			stopTime = false;
			//cameraPos = Vec3(0, 10, 10);
			prevDir = Vec3(0.01f, 0.1f, 0);
			prevMousePos = Vec2(0.5, 0.5);
			phi = 3.14f/2.0f;
			theta = 0;
			radius = 30;
            float t = radius * cos(theta);
			cameraPos = Vec3(t * cos(phi), radius * sin(theta), t * sin(phi));
		}
		virtual void onUpdate(const unsigned int deltaTime) {}
		virtual void onRender(const unsigned int deltaTime)
		{
			renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

			camera.setPosition(cameraPos);
            g->getMaterial().setProperty("cameraPos", Vec3(cameraPos));
			renderer->render(scene, camera);
			if (!stopTime)
			{
				time++;
			}
		}

		virtual void onDestroy() { delete g; }
		virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
		virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
		virtual void onKeyboardEvent(unsigned char c, int x, int y)
		{
			cout << c << " " << x << " " << y << endl;

			switch (c)
			{

			case 'j':
				time -= 5;
				break;
			case 'l':
				time += 5;
				break;
			case 'a':
				phi -= 0.1f;
				break;
			case 's':
				theta -= 0.1f;
				break;
			case 'd':
				phi += 0.1f;
				break;
			case 'w':
				theta += 0.1f;
				break;
			case 'q':
				radius += 0.5f;
				break;
			case 'e':
				radius -= 0.5f;
				break;
			default:
				stopTime = !stopTime;
				break;
			}
            float t = radius * cos(theta);
            cameraPos = Vec3(t * cos(phi), radius * sin(theta), t * sin(phi));
		}

		virtual void onMouseMove(int x, int y)
		{
			/*float nx = x / 800.0f;
			float ny = y / 800.0f;
			Vec2 delta = Vec2(nx, ny) - prevMousePos;

			Vec3 zAxis = (-cameraPos).normalize();
			Vec3 up(0, 1, 0);
			Vec3 xAxis = (zAxis * up).normalize();
			Vec3 yAxis = xAxis * zAxis;

			cameraPos -= xAxis * delta.x * 50;
			cameraPos += yAxis * delta.y * 50;

			prevMousePos = Vec2(nx, ny);*/
		}
	private:
		PerspectiveCamera camera;
		Scene scene;
		Renderer* renderer;
		CustomGeometry* g;
        CustomGeometry* environmentCube;
		ShaderMaterial* shaderMaterial;
		unsigned int time;
		bool stopTime;
		Vec3 cameraPos;
		Vec3 prevDir;
		Vec2 prevMousePos;
		float phi;
		float theta;
		float radius;
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