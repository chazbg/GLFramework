#pragma once

#pragma once
#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/TextureGenerator.hpp"
#include <GL/glew.h>
#include <vector>
#include <Core/PerspectiveCamera.hpp>
#include <Core/Scene.hpp>
#include <Core/ShaderMaterial.hpp>
#include <Core/TextureFactory.hpp>
#include <Geometry/PlaneMesh.hpp>

using namespace std;

namespace WaveDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp() : renderer(0) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer();

            TextureFactory texFactory;
            TextureGenerator gen;

            time = 0;
            stopTime = false;
            cameraPos = Vec3(0, 15, 25);
            waveMat = new OpenGLMaterial("Shaders/plane.vs", "Shaders/plane.fs");
            waveMat->addTexture(texFactory.createTexture(256, 256, 4, (unsigned char*)gen.generateOctavePerlinNoise(1, 0, 3, 0.5)));

            p = new PlaneMesh(50, 50);
            //c1->SetPosition(Vec3(0, -1, 0));
            p->setMaterial(waveMat);

            scene.add(p);

            prevMousePos = Vec2(0.5, 0.5);
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            camera.setPosition(cameraPos);

            p->SetRotation(0, time * 0.01f, 0);
            waveMat->setProperty("time", time);

            renderer->render(scene, camera);
            if (!stopTime)
            {
                time++;
            }
        }

        virtual void onDestroy()
        {
            delete renderer;
            delete p;
        }
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y)
        {
            cout << button << " " << state << " " << x << " " << y << endl;
            stopTime = !stopTime;
        }

        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {
            cout << c << " " << x << " " << y << endl;
            stopTime = !stopTime;
            Vec3 zAxis = (-cameraPos).normalize();
            Vec3 up(0, 1, 0);
            Vec3 xAxis = (zAxis * up).normalize();
            Vec3 yAxis = xAxis * zAxis;

            switch (c)
            {

            case 'j':
                time -= 5;
                break;
            case 'l':
                time += 5;
                break;
            case 'a':
                cameraPos -= xAxis;
                break;
            case 's':
                cameraPos -= yAxis;
                break;
            case 'd':
                cameraPos += xAxis;
                break;
            case 'w':
                cameraPos += yAxis;
                break;
            case 'q':
                cameraPos -= zAxis;
                break;
            case 'e':
                cameraPos += zAxis;
                break;
            default:
                stopTime = !stopTime;
                break;
            }
        }

        virtual void onMouseMove(int x, int y)
        {
            float nx = x / 800.0f;
            float ny = y / 800.0f;
            Vec2 delta = Vec2(nx, ny) - prevMousePos;

            Vec3 zAxis = (-cameraPos).normalize();
            Vec3 up(0, 1, 0);
            Vec3 xAxis = (zAxis * up).normalize();
            Vec3 yAxis = xAxis * zAxis;

            cameraPos -= xAxis * delta.x * 50;
            cameraPos += yAxis * delta.y * 50;

            prevMousePos = Vec2(nx, ny);
        }
    private:
        Renderer* renderer;
        PlaneMesh* p;
        unsigned int time;
        bool stopTime;
        Vec3 cameraPos;
        Scene scene;
        PerspectiveCamera camera;
        OpenGLMaterial* waveMat;
        Vec2 prevMousePos;
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