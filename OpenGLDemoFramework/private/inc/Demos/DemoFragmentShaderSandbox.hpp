#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/TextureGenerator.hpp"
#include <Core/DefaultCamera.hpp>
#include <Core/Scene.hpp>
#include <Core/ShaderMaterial.hpp>

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
			rectangle = new Rectangle();
			rectangle->attachTexture(Texture(256, 1, 4, (unsigned char*) gen.generateGradient()));
			rectangle->setMaterial(new ShaderMaterial("Shaders/fragmentShaderSandbox.vs", "Shaders/fragmentShaderSandbox.fs"));
			scene.add(rectangle);
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