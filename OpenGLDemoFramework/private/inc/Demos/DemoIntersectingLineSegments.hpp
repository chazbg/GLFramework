#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Geometry/LineListMesh.hpp"
#include "Geometry/PointListMesh.hpp"
#include <iostream>

namespace LIDemo
{
	class TestWindowApp : public IApplication
	{
	public:
		TestWindowApp() : renderer(0) {}
		~TestWindowApp() {}
		virtual void onInit() 
		{ 
			renderer = new Renderer();
			renderer->setDepthTest(false);

			std::vector<std::vector<Vec2>> lineSegments;

			std::vector<Vec2> segment;
			segment.push_back(Vec2(3, 6));
			segment.push_back(Vec2(10, 4));
			lineSegments.push_back(segment);

			segment.clear();
			segment.push_back(Vec2(16, 10));
			segment.push_back(Vec2(16, 5));
			lineSegments.push_back(segment);

			segment.clear();
			segment.push_back(Vec2(1, 4));
			segment.push_back(Vec2(4, 2));
			lineSegments.push_back(segment);

			segment.clear();
			segment.push_back(Vec2(8, 1));
			segment.push_back(Vec2(18, 7));
			lineSegments.push_back(segment);

			segment.clear();
			segment.push_back(Vec2(12, 6));
			segment.push_back(Vec2(14, 3));
			lineSegments.push_back(segment);

			std::vector<Vec2> intersections = GeometryAlgorithm::SweepingLineIntersection(lineSegments);

			for (unsigned int i = 0; i < intersections.size(); i++)
			{
				cout << "Intersection: " << intersections[i].toString() << endl;
			}

			for (unsigned int i = 0; i < lineSegments.size(); i++)
			{
				lines[i] = new LineListMesh(lineSegments[i]);
			}

			plm = new PointListMesh(intersections, Vec3(0, 1, 0));
		}
		virtual void onUpdate(const unsigned int deltaTime) {}
		virtual void onRender(const unsigned int deltaTime)
		{
			renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

			for (int i = 0; i < 5; i++)
			{
				lines[i]->Render();
			}

			plm->Render();
		}

		virtual void onDestroy() 
		{ 
			delete renderer;
			for (int i = 0; i < 5; i++)
			{
				delete lines[i];
			}

			delete plm;
		}
		virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
		virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
		virtual void onKeyboardEvent(unsigned char c, int x, int y) { cout << c << " " << x << " " << y << endl; }
	private:
		Renderer* renderer;
		LineListMesh* lines[5];
		PointListMesh* plm;
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