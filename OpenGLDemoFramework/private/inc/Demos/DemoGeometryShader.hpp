#pragma once

#include "Windowing/Window.hpp"
#include "Core/Scene.hpp"
#include "Demos/Demo3DBase.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

namespace GeometryShaderDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) : Demo3DBase(resolution) {}
        ~TestWindowApp() {}

        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            Demo3DBase::onRender(deltaTime);

            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            camera.setPosition(cameraPos);

            renderer->render(scene, camera);
        }

    private:

        void initMaterials()
        {
            IResourceManager& resourceManager = renderer->getResourceManager();

            //0
            materials.push_back(resourceManager.createMaterial(
                "Shaders/basicDiffuse.vs",
                "Shaders/basicDiffuse.fs"));
            initMaterialProperty(*materials[0], "diffuse", Vec3(1.0f, 1.0f, 0.0f));

            //1
            materials.push_back(resourceManager.createMaterial(
                "Shaders/basicDiffuse.vs", 
                "Shaders/basicDiffuse.fs", 
                "Shaders/basicGeometry.gs"));
            initMaterialProperty(*materials[1], "diffuse", Vec3(1.0f, 0.0f, 0.0f));
        }

        void initGeometry()
        {
            IResourceManager& rm = renderer->getResourceManager();
            meshes.push_back(new CustomGeometry(rm, "3DAssets/female_elf-3ds.3DS"));
            meshes[0]->Scale(0.1f, 0.1f, 0.1f);
            meshes[0]->Rotate(-3.14f / 2.0f, 0, 0);
            meshes[0]->Translate(0, -20, 0);

            meshes.push_back(new CustomGeometry(rm, "3DAssets/ogrehead.obj"));
            meshes[1]->Scale(7.0f, 7.0f, 7.0f);

            /*g = new CustomGeometry("3DAssets/buddha.3ds");
            g->Scale(0.01f, 0.01f, 0.01f);
            g->Rotate(-3.14f / 2.0f, 0, 0);
            g->Translate(0, 1, 0);*/

            meshes.push_back(new CustomGeometry(rm, "3DAssets/hair5.obj"));
            meshes[2]->Scale(87, 85, 77);
            meshes[2]->Translate(0, -55, 0);

            meshes.push_back(new CustomGeometry(rm, "3DAssets/Sphere.3ds"));
            meshes[3]->Scale(0.5, 0.5, 0.5);

            currentMesh = meshes[0];
            currentMesh->setMaterial(materials[0]);

            meshes.push_back(new CustomGeometry(rm, "3DAssets/female_elf-3ds.3DS"));
            meshes[4]->Scale(0.1f, 0.1f, 0.1f);
            meshes[4]->Rotate(-3.14f / 2.0f, 0, 0);
            meshes[4]->Translate(0, -20, 0);
            meshes[4]->setMaterial(materials[1]);

            scene.add(currentMesh);
            scene.add(meshes[4]);
        }
        
        CustomGeometry* currentMesh;
        vector<CustomGeometry*> meshes;
        vector<IMaterial*> materials;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1280;
        params.height = 720;
        params.posX = 0;
        params.posY = 0;
        params.name = "GeometryShader";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}