#pragma once
#include "Texture.hpp"
#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Rectangle.hpp"
#include "TextureGenerator.hpp"
#include <GL/glew.h>
namespace TexDemo
{
	Rectangle* r;
	bool stopTime;
	unsigned int time;

	void KeyboardCallback(unsigned char c, int x, int y)
	{
		cout << c << " " << x << " " << y << endl;
		stopTime = !stopTime;
	}

	void MouseCallback(int button, int state, int x, int y)
	{
		cout << button << " " << state << " " << x << " " << y << endl;
		stopTime = !stopTime;
	}

	void RenderDemoTex()
	{
		GLWrapper::ClearWindow();

		if (!stopTime)
		{
			r->SetTime(time);
			time++;
		}

		r->Render();

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
		
		stopTime = false;
		time = 0;

		r = new Rectangle();
		r->attachTexture(Texture(800, 800, 4, 0));
		r->attachShaders("Shaders/noTex.vs", "Shaders/noTex.fs");
		GLUTWrapper::RenderLoop();
	}

}