#pragma once

#include "Windowing/Window.hpp"
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
		virtual void onUpdate(const unsigned int deltaTime) { cout << "onUpdate: " << deltaTime << endl; }
		virtual void onRender(const unsigned int deltaTime) { cout << "onRender: " << deltaTime << endl; }
		virtual void onDestroy() { cout << "onDestroy" << endl; }
		virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
	};

	void main()
	{
		WindowParameters params;
		params.width = 800;
		params.height = 800;
		params.posX = 100;
		params.posY = 100;
		params.clearColor = Vec4(0.0f, 0.0f, 0.5f, 1.0f);
		params.name = "TestWindow";
		TestWindowApp app;
		Window window(params, app);
		params.posX += 50;
		Window window1(params, app);
		params.posX += 50;
		Window window2(params, app);
		params.posX += 50;
		Window window3(params, app);
		window.startRenderLoop();
	}
}