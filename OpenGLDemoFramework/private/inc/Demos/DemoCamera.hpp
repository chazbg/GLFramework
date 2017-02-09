#pragma once

#include "Math/GeometryAlgorithm.hpp"
#include "Demos/Demo3DBase.hpp"
#include "Core/DefaultCamera.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

namespace CameraDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) : Demo3DBase(resolution) {}

        virtual void onInit()
        {
            Demo3DBase::onInit();
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            Demo3DBase::onRender(deltaTime);

            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            renderer->render(scene, camera);
        }

        virtual void onMouseMove(int x, int y)
        {
            // TODO:

            Demo3DBase::onMouseMove(x, y);
        }
    private:

        virtual void initMaterials()
        {
            IResourceManager& resourceManager = renderer->getResourceManager();

            //0
            materials.push_back(resourceManager.createMaterial("Shaders/basic.vs", "Shaders/basic.fs"));
        }

        virtual void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/female_elf-3ds.3DS"));
            meshes[0]->scale(Vec3(0.1f));
            meshes[0]->rotate(Vec3(-3.14f / 2.0f, 0.0f, 0.0f));
            meshes[0]->translate(Vec3(-1.0f, 5.0f, 2.0f));
            meshes[0]->setMaterial(materials[0]);

            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/Sphere.3ds"));
            meshes[1]->scale(Vec3(0.5f));
            meshes[1]->translate(Vec3(5.0f));
            meshes[1]->setMaterial(materials[0]);

            scene.add(meshes[0]);
            scene.add(meshes[1]);
        }

        vector<std::shared_ptr<MeshNode>> meshes;
        vector<IMaterial*> materials;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1600;
        params.height = 900;
        params.posX = 0;
        params.posY = 0;
        params.name = "Camera";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
            static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}