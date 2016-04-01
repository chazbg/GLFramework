#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/TextureGenerator.hpp"
#include <Core/DefaultCamera.hpp>
#include <Core/Scene.hpp>
#include <Core/ShaderMaterial.hpp>
#include <Core/TextureFactory.hpp>
#include <iostream>
using namespace std;

namespace FragmentShaderSandboxDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp() : renderer(0) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer(Vec2(1200, 600));
            TextureGenerator gen;
            TextureFactory texFactory;
            shaderMaterial = new ShaderMaterial("Shaders/fragmentShaderSandbox.vs", "Shaders/raymarching0.fs");
            shaderMaterial->setProperty("sampler0", 0);
            shaderMaterial->setProperty("sampler1", 1);
            shaderMaterial->setProperty("sampler2", 2);
            shaderMaterial->addTexture(texFactory.createTexture(256, 1, 4, (unsigned char*)gen.generateGradient()));
            shaderMaterial->addTexture(texFactory.createTexture(256, 256, 4, (unsigned char*)gen.generatePerlinNoise(0.5)));
            shaderMaterial->addTexture(texFactory.createTexture(256, 256, 4, (unsigned char*)gen.generatePerlinNoise(1)));
            rectangle = new Rectangle();
            rectangle->setMaterial(shaderMaterial);
            scene.add(rectangle);
            time = 0;
            stopTime = false;
            //cameraPos = Vec3(0, 5, 5);
            prevDir = Vec3(0.01f, 0.1f, 0);
            prevMousePos = Vec2(0.5, 0.5);
            phi = 0;
            theta = 0;
            radius = 10;
            cameraPos = Vec3(radius * cos(theta) * sin(phi), radius * sin(theta) * sin(phi), radius * cos(phi));
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            shaderMaterial->setProperty("time", (float) time);
            shaderMaterial->setProperty("cameraPos", cameraPos);
            renderer->render(scene, camera);
            if (!stopTime)
            {
                time++;
            }
        }

        virtual void onDestroy() { cout << "onDestroy" << endl; }
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {
            cout << c << " " << x << " " << y << endl;

            switch (c)
            {

            case 'j':
                time -= 5;
                break;
            case 'l':
                time += 5;
                break;
            case 'a':
                phi -= 0.1f;
                break;
            case 's':
                theta -= 0.1f;
                break;
            case 'd':
                phi += 0.1f;
                break;
            case 'w':
                theta += 0.1f;
                break;
            case 'q':
                radius += 0.1f;
                break;
            case 'e':
                radius -= 0.1f;
                break;
            default:
                stopTime = !stopTime;
                break;
            }
            cameraPos = Vec3(radius * sin(theta) * sin(phi), radius * cos(phi), radius * cos(theta) * sin(phi));
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
        DefaultCamera camera;
        Scene scene;
        Renderer* renderer;
        Rectangle* rectangle;
        ShaderMaterial* shaderMaterial;
        unsigned int time;
        bool stopTime;
        Vec3 cameraPos;
        Vec3 prevDir;
        Vec2 prevMousePos;
        float phi;
        float theta;
        float radius;
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