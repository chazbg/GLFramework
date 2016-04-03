#pragma once

#include <Windowing/WindowFactory.hpp>
#include <iostream>
using namespace std;

namespace WindowDemo
{
	class TestWindowApp : public IApplication
	{
	public:
		TestWindowApp() {}
		~TestWindowApp() {}
		virtual void onInit() { cout << "onInit" << endl; }
		virtual void onUpdate(const unsigned int deltaTime) { /*cout << "onUpdate: " << deltaTime << endl;*/ }
		virtual void onRender(const unsigned int deltaTime) { /*cout << "onRender: " << deltaTime << endl;*/ }
		virtual void onDestroy() { cout << "onDestroy" << endl; }
		virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
		virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
		virtual void onKeyboardEvent(unsigned char c, int x, int y) { cout << c << " " << x << " " << y << endl; }
        virtual void onMouseMove(int x, int y) {}
	};

	void main()
	{
		WindowParameters params;
		params.width = 800;
		params.height = 800;
		params.posX = 0;
		params.posY = 0;
		params.name = "TestWindow";
		TestWindowApp app;
        WindowFactory windowFactory;
        IWindow* window = windowFactory.createWindow(params, app);

		window->startRenderLoop();
	}
}