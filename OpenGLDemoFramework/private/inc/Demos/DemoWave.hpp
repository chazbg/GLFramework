#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/TextureGenerator.hpp"
#include "Core/PerspectiveCamera.hpp"
#include "Core/Scene.hpp"
#include "Demos/Demo3DBase.hpp"

using namespace std;

namespace WaveDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) : Demo3DBase(resolution) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            Demo3DBase::onInit();
        }

        virtual void onRender(const unsigned int deltaTime)
        {
            Demo3DBase::onRender(deltaTime);

            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            //p->Rotate(0, 0.01f, 0);
            waveMat->setProperty(timeProperty, time);

            renderer->render(scene, camera);
        }

    private:
        virtual void initTextures()
        {
            IResourceManager& rm = renderer->getResourceManager();
            TextureGenerator gen;
            texture = rm.createTexture(256, 256, 4,
                reinterpret_cast<unsigned char*>(gen.generateOctavePerlinNoise(1, 0, 3, 0.5)));
        }

        virtual void initMaterials()
        {
            IResourceManager& resourceManager = renderer->getResourceManager();

            waveMat = resourceManager.createMaterial("Shaders/plane.vs", "Shaders/plane.fs");
            waveMat->addTexture(texture);
            initMaterialProperty(*waveMat, "sampler", 0);
            waveMat->getProperty("time", timeProperty);
        }

        virtual void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            p = geometryFactory.createPlaneMesh(50, 50);
            p->setMaterial(waveMat);
            scene.add(p.get());
        }

        shared_ptr<PlaneMesh> p;
        IMaterial* waveMat;
        ITexture* texture;
        UintPropertySharedPtr timeProperty;
    };

    void main()
    {
        WindowParameters params;
        params.width = 800;
        params.height = 800;
        params.posX = 100;
        params.posY = 100;
        params.name = "Wave";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}