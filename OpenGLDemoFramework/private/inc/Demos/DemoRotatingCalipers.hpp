#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Geometry/LineListMesh.hpp"
#include "Geometry/PointListMesh.hpp"
#include <iostream>

namespace RCDemo
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

			time = 0;

			std::vector<Vec2> inputPolygon;
			std::vector<std::vector<Vec2>> antipodPoints;
			inputPolygon.push_back(Vec2(7, 4));
			inputPolygon.push_back(Vec2(10, 7));
			inputPolygon.push_back(Vec2(8, 10));
			inputPolygon.push_back(Vec2(4, 11));
			inputPolygon.push_back(Vec2(1, 5));
			inputPolygon.push_back(Vec2(2, 2));

			antipodPoints = GeometryAlgorithm::RotatingCalipers(inputPolygon);

			llm = new LineListMesh(inputPolygon, Vec3(1, 1, 1), 4.0f);
			std::vector<Vec2> maxDist = antipodPoints[0];
			for (unsigned int i = 0; i < antipodPoints.size(); i++)
			{
				float dist = antipodPoints[i][0].distanceTo(antipodPoints[i][1]);
				if (dist > maxDist[0].distanceTo(maxDist[1]))
				{
					maxDist = antipodPoints[i];
				}
			}

			std::cout << "Pairs: " << antipodPoints.size() << std::endl;
			std::cout << "Max Distance points: " << maxDist[0].toString() << ", " << maxDist[1].toString() << std::endl;
			std::cout << "Max Distance: " << maxDist[0].distanceTo(maxDist[1]) << std::endl;
			maxDistance = new LineListMesh(maxDist, Vec3(1, 1, 0), 2.0f);
			for (unsigned int i = 0; i < antipodPoints.size(); i++)
			{
				plm.push_back(new PointListMesh(antipodPoints[i], Vec3(0.7f, 0, 0), 10.0f));
			}
		}
		virtual void onUpdate(const unsigned int deltaTime) {}
		virtual void onRender(const unsigned int deltaTime)
		{
			renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

			llm->Render();
			maxDistance->Render();

			plm[((int)(time * 0.01)) % plm.size()]->Render();
			time++;
		}

		virtual void onDestroy()
		{
			delete renderer;
			delete llm;
			delete maxDistance;
			for (unsigned int i = 0; i < plm.size(); i++)
			{
				delete plm[i];
			}
		}
		virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
		virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
		virtual void onKeyboardEvent(unsigned char c, int x, int y) { cout << c << " " << x << " " << y << endl; }
	private:
		Renderer* renderer;
		LineListMesh* llm;
		LineListMesh* maxDistance;
		std::vector<PointListMesh*> plm;
		int time;
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