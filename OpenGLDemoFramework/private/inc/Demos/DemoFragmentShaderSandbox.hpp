#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/TextureGenerator.hpp"
#include "Core/DefaultCamera.hpp"
#include "Core/Scene.hpp"
#include <iostream>
using namespace std;

namespace FragmentShaderSandboxDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp(const Vec2 resolution) : resolution(resolution), renderer(0) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer(resolution);
            TextureGenerator gen;
            IResourceManager& rm = renderer->getResourceManager();
            IGeometryFactory& gf = renderer->getGeometryFactory();

            shaderMaterial = rm.createMaterial("Shaders/fragmentShaderSandbox.vs", "Shaders/raycast.fs");
            initMaterialProperty(*shaderMaterial, "sampler0", 0);
            initMaterialProperty(*shaderMaterial, "sampler1", 1);
            shaderMaterial->getProperty("time", timeProperty);

            shaderMaterial->addTexture(rm.createTexture(256, 1, 4, (unsigned char*)gen.generateGradient()));
            shaderMaterial->addTextureCubemap(rm.createTextureCubemap(
                "Images/cubemap_1_light/posz.png",
                "Images/cubemap_1_light/negz.png",
                "Images/cubemap_1_light/posy.png",
                "Images/cubemap_1_light/negy.png",
                "Images/cubemap_1_light/posx.png",
                "Images/cubemap_1_light/negx.png"
                ));
            
            auto rectangle = gf.createRectangle();
            rectangle->setMaterial(shaderMaterial);
            scene.add(rectangle);
            time = 0;
            stopTime = false;
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

            shaderMaterial->setProperty(timeProperty, static_cast<float>(time));
            shaderMaterial->setProperty(cameraPosProperty, cameraPos);
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
            float nx = x / resolution.x;
            float ny = y / resolution.y;
            Vec2 delta = Vec2(nx, ny)- prevMousePos;

            Vec3 zAxis = (-cameraPos).normalize();
            Vec3 up(0, 1, 0);
            Vec3 xAxis = zAxis.cross(up).normalize();
            Vec3 yAxis = xAxis.cross(zAxis);

            cameraPos -= xAxis * delta.x * 50;
            cameraPos += yAxis * delta.y * 50;

            prevMousePos = Vec2(nx, ny);
        }
    private:
        template <typename T>
        void initMaterialProperty(IMaterial& material, const std::string propertyName, const T& value);

        Vec2 resolution;
        DefaultCamera camera;
        Scene scene;
        Renderer* renderer;
        IMaterial* shaderMaterial;
        unsigned int time;
        bool stopTime;
        Vec3 cameraPos;
        Vec2 prevMousePos;
        float phi;
        float theta;
        float radius;
        FloatPropertySharedPtr timeProperty;
        Vec3PropertySharedPtr cameraPosProperty;
    };

    void main()
    {
        WindowParameters params;
        params.width = 800;
        params.height = 800;
        params.posX = 100;
        params.posY = 100;
        params.name = "TestWindow";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
    template<typename T>
    inline void TestWindowApp::initMaterialProperty(IMaterial & material, const std::string propertyName, const T & value)
    {
        std::shared_ptr<IMaterialProperty<T>> p;
        material.getProperty(propertyName, p);
        if (p != 0)
        {
            material.setProperty(p, value);
        }
        else
        {
            std::cout << "Couldn't find property \"" << propertyName << "\"" << std::endl;
        }
    }
}