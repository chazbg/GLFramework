#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include <iostream>
using namespace std;

namespace RendererDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp(const Vec2 resolution) : resolution(resolution), renderer(0) 
        {

        }
        ~TestWindowApp() {}
        virtual void onInit() 
        { 
            cout << "onInit" << endl; 
            renderer = new Renderer(resolution);
        }

        virtual void onUpdate(const unsigned int deltaTime) { /*cout << "onUpdate: " << deltaTime << endl;*/ }
        virtual void onRender(const unsigned int deltaTime) { renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f)); }
        virtual void onDestroy() { cout << "onDestroy" << endl; }
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
        virtual void onKeyboardEvent(unsigned char c, int x, int y) { cout << c << " " << x << " " << y << endl; }
        virtual void onMouseMove(int x, int y) { }
    private:
        Vec2 resolution;
        Renderer* renderer;
    };

    void main()
    {
        WindowParameters params;
        params.width = 800;
        params.height = 800;
        params.posX = 100;
        params.posY = 100;
        params.name = "TestRenderer";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}