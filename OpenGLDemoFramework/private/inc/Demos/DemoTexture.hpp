#pragma once
#include "Core/Texture.hpp"
#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/TextureGenerator.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Core/FrameBuffer.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Geometry/Triangle.hpp"
#include <GL/glew.h>
#include <vector>
#include <Core/PerspectiveCamera.hpp>
#include <Core/Scene.hpp>
#include <Core/ShaderMaterial.hpp>
#include <Core/TextureFactory.hpp>

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

			time = 0;
			stopTime = false;
			cameraPos = Vec3(0, 15, 25);
			meshPos = Vec3(-10.0, 3, 0);
			prevDir = Vec3(0.01f, 0.1f, 0);
            cubeMat = new ShaderMaterial("Shaders/cube.vs", "Shaders/cube.fs");
			c1 = new BlockMesh(2.0f, 5.0f, 5.0f);
			c1->SetPosition(Vec3(0, -1, 0));
            c1->setMaterial(cubeMat);
			meshes.push_back(new BlockMesh(2.0f, 5.0f, 5.0f));
			meshes[0]->SetPosition(Vec3(-2, -1, -6));
            meshes[0]->setMaterial(cubeMat);
			meshes.push_back(new BlockMesh(2.0f, 5.0f, 5.0f));
			meshes[1]->SetPosition(Vec3(2, -1, 6));
            meshes[1]->setMaterial(cubeMat);
			meshes.push_back(new BlockMesh(20.0f, 0.2f, 20.0f));
			meshes[2]->SetPosition(Vec3(0, -3, 0));
            meshes[2]->setMaterial(cubeMat);
			meshes.push_back(new BlockMesh(20.0f, 20.0f, 0.2f));
			meshes[3]->SetPosition(Vec3(0, -3, -10));
            meshes[3]->setMaterial(cubeMat);
            meshes.push_back(new BlockMesh(0.5f, 0.5f, 0.5f));
            meshes[4]->setMaterial(cubeMat);
			//meshes[4]->SetPosition(Vec3(0, 10, 0));

            scene.add(c1);
            //scene.add(r);
            scene.add(meshes[0]);
            scene.add(meshes[1]);
            scene.add(meshes[2]);
            scene.add(meshes[3]);
            scene.add(meshes[4]);
		}
		virtual void onUpdate(const unsigned int deltaTime) {}
		virtual void onRender(const unsigned int deltaTime)
		{
			renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            camera.setPosition(cameraPos);

			prevDir += Vec3(0, -0.001f, 0);
			meshPos += prevDir;
			meshes[4]->SetPosition(meshPos);

            c1->SetRotation(0, time * 0.01f, 0);
            meshes[0]->SetRotation(0, time * 0.01f, 0);
            meshes[1]->SetRotation(0, time * 0.01f, 0);
            renderer->render(scene, camera);
			if (!stopTime)
			{
				time++;
			}
		}

		virtual void onDestroy()
		{
			delete renderer;
			delete c1;
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
			case 'r':
				meshPos = Vec3(-10.0, 3, 0);
				prevDir = Vec3(0.01f, 0.1f, 0);
				break;
			default:
				stopTime = !stopTime;
				break;
			}
		}
	private:
		Renderer* renderer;
		BlockMesh* c1;
		vector<BlockMesh*> meshes;
		unsigned int time;
		bool stopTime;
		Vec3 cameraPos;
		Vec3 meshPos;
		Vec3 prevDir;
        Scene scene;
        PerspectiveCamera camera;
        ShaderMaterial* cubeMat;
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