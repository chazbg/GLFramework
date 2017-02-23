#pragma once

#include "Demos/Demo3DBase.hpp"

using namespace std;

namespace FanDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) : Demo3DBase(resolution) {}

        virtual void onInit()
        {
            Demo3DBase::onInit();

            renderer->setAlphaBlending(true, BlendMode::Normal);
        }

        virtual void onRender(const unsigned int deltaTime)
        {
            Demo3DBase::onRender(deltaTime);

            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            materials[0]->setProperty(timeProperty, abs(sin(time * 0.016f * 0.25f)));

            for (int i = 0; i < 18; i++)
            {
                materials[0]->setProperty(index, float(i));
                renderer->render(scene, camera);
            }
        }
    private:

        virtual void initMaterials()
        {
            IResourceManager& resourceManager = renderer->getResourceManager();

            //0
            materials.push_back(resourceManager.createMaterial("Shaders/FanAnimation/shader.vs", "Shaders/FanAnimation/shader.fs"));
            materials[0]->getProperty("index", index);
            materials[0]->getProperty("time", timeProperty);
            initMaterialProperty(*materials[0], "wingCount", 18.0f);
        }

        virtual void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/wing.3ds"));
            meshes[0]->scale(Vec3(2.0f));
            meshes[0]->setMaterial(materials[0]);

            scene.add(meshes[0]);
        }

        vector<MeshNodeSharedPtr> meshes;
        vector<IMaterial*>        materials;
        FloatPropertySharedPtr    index;
        FloatPropertySharedPtr    timeProperty;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1600;
        params.height = 900;
        params.posX = 0;
        params.posY = 0;
        params.name = "Fan";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
            static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}