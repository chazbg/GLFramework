#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Geometry/LineListMesh.hpp"
#include "Geometry/PointListMesh.hpp"

namespace GHSCDemo
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

			plm1->Render();
			plm2->Render();
		}

		virtual void onDestroy() { cout << "onDestroy" << endl; }
		virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
		virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
		virtual void onKeyboardEvent(unsigned char c, int x, int y) { cout << c << " " << x << " " << y << endl; }
		void setRenderer(Renderer* renderer)
		{
			this->renderer = renderer;

			std::vector<Vec2> inputPolygon, convexHull;

			//inputPolygon.push_back(Vec2(1, 2));
			//inputPolygon.push_back(Vec2(1.5, 3));
			//inputPolygon.push_back(Vec2(2, 9));
			//inputPolygon.push_back(Vec2(4, 1));
			//inputPolygon.push_back(Vec2(4, 8));
			//inputPolygon.push_back(Vec2(4, 10));
			//inputPolygon.push_back(Vec2(6, 4));
			//inputPolygon.push_back(Vec2(7.5, 6));

			inputPolygon.push_back(Vec2(1, 4));
			inputPolygon.push_back(Vec2(4, 2));
			inputPolygon.push_back(Vec2(2, 9));
			inputPolygon.push_back(Vec2(4, 1));
			inputPolygon.push_back(Vec2(6, 10));
			inputPolygon.push_back(Vec2(16, 5));
			inputPolygon.push_back(Vec2(18, 7));
			inputPolygon.push_back(Vec2(7.5, 6));
			inputPolygon.push_back(Vec2(11, 3));
			inputPolygon.push_back(Vec2(10, 7));

			convexHull = GeometryAlgorithm::ConvexHullGraham(inputPolygon);

			printf("ConvexHullGraham points:\n");
			for (unsigned int i = 0; i < convexHull.size(); i++)
			{
				printf("%s\n", convexHull[i].toString().c_str());
			}

			plm1 = new PointListMesh(inputPolygon, Vec3(0, 1, 1), 6.0f);
			plm2 = new LineListMesh(convexHull, Vec3(1, 1, 0), 3.0f);
		}
	private:
		Renderer* renderer;
		PointListMesh* plm1;
		LineListMesh* plm2;
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