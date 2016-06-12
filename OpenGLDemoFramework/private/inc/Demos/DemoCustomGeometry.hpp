#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/TextureGenerator.hpp"
#include "Demos/Demo3DBase.hpp"
#include <iostream>
using namespace std;

namespace CustomGeometryDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) :
            Demo3DBase(resolution) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            Demo3DBase::onInit();

            IResourceManager& rm = renderer->getResourceManager();
            IGeometryFactory& gf = renderer->getGeometryFactory();

            shaderMaterial = rm.createMaterial("Shaders/basic.vs", "Shaders/basic.fs");
            shaderMaterial->getProperty("time", timeProperty);
            shaderMaterial->getProperty("cameraPos", cameraPosProperty);

            g = gf.createCustomGeometry("3DAssets/female_elf-3ds.3DS");
            g->Scale(0.1f, 0.1f, 0.1f);
            g->Rotate(-3.14f / 2.0f, 0, 0);
            g->Translate(0, -20, 0);
            g->setMaterial(shaderMaterial);
            
            scene.add(g.get());
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            Demo3DBase::onRender(deltaTime);

            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            shaderMaterial->setProperty(timeProperty, static_cast<float>(time));
            shaderMaterial->setProperty(cameraPosProperty, cameraPos);
            renderer->render(scene, camera);
        }

    private:
        std::shared_ptr<CustomGeometry> g;
        IMaterial* shaderMaterial;
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
        params.name = "CustomGeometry";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}