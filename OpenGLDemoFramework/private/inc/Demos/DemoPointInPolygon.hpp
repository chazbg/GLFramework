#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Geometry/LineListMesh.hpp"
#include "Geometry/PointListMesh.hpp"

namespace PIPDemo
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

            std::vector<Vec2> testPoints;
            std::vector<Vec2> polygon;
            std::vector<Vec2> pointsInside;
            std::vector<Vec2> pointsOutside;
            bool res;

            polygon.push_back(Vec2(1, 1));
            polygon.push_back(Vec2(3, 5));
            polygon.push_back(Vec2(5, 4));
            polygon.push_back(Vec2(9, 7));
            polygon.push_back(Vec2(11, 3));
            polygon.push_back(Vec2(11, 1));
            polygon.push_back(Vec2(8, 2));

            testPoints.push_back(Vec2(4, 2));
            testPoints.push_back(Vec2(10, 3.5));

            for (unsigned int i = 0; i < testPoints.size(); i++)
            {
                res = GeometryAlgorithm::PointInPolygon(testPoints[i], polygon);

                if (res)
                {
                    pointsInside.push_back(testPoints[i]);
                }
                else
                {
                    pointsOutside.push_back(testPoints[i]);
                }

                printf("TestPointInPolygon result for point: %s -> %d\n", testPoints[i].toString().c_str(), res);
            }

            plm1 = new LineListMesh(polygon, Vec3(0, 1, 1), 6.0f);
            plm2 = new PointListMesh(pointsInside, Vec3(1, 1, 0), 3.0f);
            plm3 = new PointListMesh(pointsOutside, Vec3(1, 0, 0), 1.0f);
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            plm1->Render();
            plm2->Render();
            plm3->Render();
        }

        virtual void onDestroy()
        {
            delete renderer;
            delete plm1;
            delete plm2;
            delete plm3;
        }
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
        virtual void onKeyboardEvent(unsigned char c, int x, int y) { cout << c << " " << x << " " << y << endl; }
    private:
        Renderer* renderer;
        LineListMesh* plm1;
        PointListMesh* plm2;
        PointListMesh* plm3;
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