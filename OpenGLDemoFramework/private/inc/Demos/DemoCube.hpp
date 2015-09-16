#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/Cube.hpp"

namespace CubeDemo
{
	class TestWindowApp : public IApplication
	{
	public:
		TestWindowApp() : renderer(0) {}
		~TestWindowApp() {}
		virtual void onInit() { cout << "onInit" << endl; }
		virtual void onUpdate(const unsigned int deltaTime) {}
		virtual void onRender(const unsigned int deltaTime)
		{
			renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

			cube->Render();
		}

		virtual void onDestroy() { cout << "onDestroy" << endl; }
		virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
		virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
		virtual void onKeyboardEvent(unsigned char c, int x, int y) { cout << c << " " << x << " " << y << endl; }
		virtual void onMouseMove(int x, int y) { }
		void setRenderer(Renderer* renderer)
		{
			this->renderer = renderer;
			cube = new Cube();
		}
	private:
		Renderer* renderer;
		Cube* cube;
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
		Renderer renderer;
		app.setRenderer(&renderer);
		window.startRenderLoop();
	}
}