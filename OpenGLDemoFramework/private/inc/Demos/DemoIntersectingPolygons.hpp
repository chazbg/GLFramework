#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Geometry/LineListMesh.hpp"

namespace PIDemo
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

            std::vector<Vec2> inputPolygon1;
            std::vector<Vec2> inputPolygon2;
            std::vector<Vec2> outputPolygon;

            inputPolygon1.push_back(Vec2(2, 2));
            inputPolygon1.push_back(Vec2(5, 2));
            inputPolygon1.push_back(Vec2(8, 6));
            inputPolygon1.push_back(Vec2(6, 8));
            inputPolygon1.push_back(Vec2(2, 9));
            inputPolygon1.push_back(Vec2(1, 6));

            inputPolygon2.push_back(Vec2(0, 4.5));
            inputPolygon2.push_back(Vec2(3, 3));
            inputPolygon2.push_back(Vec2(7, 3));
            inputPolygon2.push_back(Vec2(9, 4));
            inputPolygon2.push_back(Vec2(10, 8));
            inputPolygon2.push_back(Vec2(4, 7));

            outputPolygon = GeometryAlgorithm::IntersectPolygons(inputPolygon1, inputPolygon2);

            p1 = new LineListMesh(inputPolygon1, Vec3(1, 0, 0), 4.0f);
            p2 = new LineListMesh(inputPolygon2, Vec3(1, 1, 0), 4.0f);
            p3 = new LineListMesh(outputPolygon, Vec3(0, 1, 1), 4.0f);
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            p1->Render();
            p2->Render();
            p3->Render();
        }

        virtual void onDestroy()
        {
            delete renderer;
            delete p1;
            delete p2;
            delete p3;
        }
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
        virtual void onKeyboardEvent(unsigned char c, int x, int y) { cout << c << " " << x << " " << y << endl; }
    private:
        Renderer* renderer;
        LineListMesh* p1;
        LineListMesh* p2;
        LineListMesh* p3;
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