#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/TextureGenerator.hpp"
#include <Core/DefaultCamera.hpp>
#include <Core/Scene.hpp>
#include <Core/ShaderMaterial.hpp>
#include <Core/TextureFactory.hpp>
#include <iostream>
using namespace std;

namespace FragmentShaderSandboxDemo
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
			TextureFactory texFactory;
			shaderMaterial = new ShaderMaterial("Shaders/fragmentShaderSandbox.vs", "Shaders/fragmentShaderSandbox.fs");
            shaderMaterial->setProperty("sampler0", 0);
            shaderMaterial->setProperty("sampler1", 1);
            shaderMaterial->setProperty("sampler2", 2);
			shaderMaterial->addTexture(texFactory.createTexture(256, 1, 4, (unsigned char*)gen.generateGradient()));
            shaderMaterial->addTexture(texFactory.createTexture(256, 256, 4, (unsigned char*)gen.generatePerlinNoise(0.5)));
            shaderMaterial->addTexture(texFactory.createTexture(256, 256, 4, (unsigned char*)gen.generatePerlinNoise(1)));
			rectangle = new Rectangle();
			rectangle->setMaterial(shaderMaterial);
			scene.add(rectangle);
            time = 0;
		}
		virtual void onUpdate(const unsigned int deltaTime) {}
		virtual void onRender(const unsigned int deltaTime) 
		{ 
			renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f)); 

			rectangle->SetTime(rectangle->GetTime() + 1);
			renderer->render(scene, camera);
		}

		virtual void onDestroy() { cout << "onDestroy" << endl; }
		virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
		virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
		virtual void onKeyboardEvent(unsigned char c, int x, int y) { cout << c << " " << x << " " << y << endl; }
	private:
		DefaultCamera camera;
		Scene scene;
		Renderer* renderer;
		Rectangle* rectangle;
		ShaderMaterial* shaderMaterial;
        unsigned int time;
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