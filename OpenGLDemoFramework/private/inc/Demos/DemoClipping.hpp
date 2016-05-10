#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Geometry/LineListMesh.hpp"

namespace CLPDemo
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
            plm3->Render();
        }

        virtual void onDestroy() { cout << "onDestroy" << endl; }
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
        virtual void onKeyboardEvent(unsigned char c, int x, int y) { cout << c << " " << x << " " << y << endl; }
        void setRenderer(Renderer* renderer)
        {
            this->renderer = renderer;
            renderer->setDepthTest(false);

            std::vector<Vec2> inputPolygon;
            std::vector<Vec2> clippingPolygon;
            std::vector<Vec2> outputPolygon;
            inputPolygon.push_back(Vec2(4, 1));
            inputPolygon.push_back(Vec2(7, 3));
            inputPolygon.push_back(Vec2(9, 15));
            inputPolygon.push_back(Vec2(15, 5));
            inputPolygon.push_back(Vec2(10, 6));
            inputPolygon.push_back(Vec2(7, 5));

            clippingPolygon.push_back(Vec2(3, 3));
            clippingPolygon.push_back(Vec2(6, 1));
            clippingPolygon.push_back(Vec2(11, 1));
            clippingPolygon.push_back(Vec2(13.5, 3));
            clippingPolygon.push_back(Vec2(8.5, 8.5));

            outputPolygon = GeometryAlgorithm::Clip(inputPolygon, clippingPolygon);
            printf("Sutherland-Hodgeman clipping points:\n");
            for (unsigned int i = 0; i < outputPolygon.size(); i++)
            {
                printf("%s\n", outputPolygon[i].toString().c_str());
            }

            plm1 = new LineListMesh(inputPolygon, Vec3(0, 1, 1), 6.0f);
            plm2 = new LineListMesh(outputPolygon, Vec3(1, 1, 0), 3.0f);
            plm3 = new LineListMesh(clippingPolygon, Vec3(1, 0, 0), 1.0f);
        }
    private:
        Renderer* renderer;
        LineListMesh* plm1;
        LineListMesh* plm2;
        LineListMesh* plm3;
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