#pragma once

#include "GLUTWrapper.hpp"
#include "GLWrapper.hpp"
#include "Cube.hpp"

namespace CubeDemo
{

	Cube* c;

	void RenderDemoCube()
	{
		GLWrapper::ClearWindow();

		c->Render();

		GLUTWrapper::UpdateFrame();
		GLUTWrapper::RequestNewFrame();
	}

	void DemoCube()
	{
		GLUTWrapper::InitWindow(&RenderDemoCube);
		GLWrapper::InitRenderer();
		c = new Cube();
		GLUTWrapper::RenderLoop();
	}

}