#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Core/Scene.hpp"
#include "Core/PerspectiveCamera.hpp"
#include "Demos/Demo3DBase.hpp"

namespace CubeDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) : 
            Demo3DBase(resolution),
            cubesCount(30) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            Demo3DBase::onInit();

            cubeVertices[0] = Vec3(0, 0, 0);
            cubeVertices[1] = Vec3(1, 0, 0);
            cubeVertices[2] = Vec3(1, 1, 0);
            cubeVertices[3] = Vec3(1, 0, 1);
            cubeVertices[4] = Vec3(1, 1, 1);
            cubeVertices[5] = Vec3(0, 1, 0);
            cubeVertices[6] = Vec3(0, 1, 1);
            cubeVertices[7] = Vec3(0, 0, 1);

            barycentricCoords[0] = 1.0f;
            barycentricCoords[1] = 0.0f;
            barycentricCoords[2] = 0.0f;
            barycentricCoords[3] = 0.0f;
            barycentricCoords[4] = 0.0f;
            barycentricCoords[5] = 0.0f;
            barycentricCoords[6] = 0.0f;
            barycentricCoords[7] = 0.0f;

            interpolationSteps = 30;
            interpolator = 0;
        }

        virtual void onRender(const unsigned int deltaTime)
        {
            Demo3DBase::onRender(deltaTime);

            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            if (!stopTime)
            {
                float delta = 1.0f / interpolationSteps;
                int index = (interpolator / interpolationSteps) % 8;
                Vec3 newPos = cubeVertices[index] * barycentricCoords[index] * 5.0f +
                    cubeVertices[(index + 1) % 8] * barycentricCoords[(index + 1) % 8] * 5.0f;

                for (unsigned int i = cubesCount - 1; i >= 1; i--)
                {
                    Vec3 translation = cubes[i - 1]->getPosition() - cubes[i]->getPosition();
                    shared_ptr<BlockMesh> cube = cubes[i];
                    cube->Translate(translation.x, translation.y, translation.z);
                }

                shared_ptr<BlockMesh> cube = cubes[0];
                Vec3 translation = newPos - cube->getPosition();
                cube->Translate(translation.x, translation.y, translation.z);

                interpolator++;
                barycentricCoords[index] -= delta;
                barycentricCoords[(index + 1) % 8] += delta;
            }

            renderer->render(scene, camera);
        }

    private:
        virtual void initTextures()
        {
            IResourceManager& rm = renderer->getResourceManager();

            textures.push_back(rm.createTexture("Images/pattern_133/diffuse.png"));
        }

        virtual void initMaterials()
        {
            IResourceManager& resourceManager = renderer->getResourceManager();

            //0
            materials.push_back(resourceManager.createMaterial(
                "Shaders/basicDiffuse.vs",
                "Shaders/basicDiffuse.fs"));
            materials[0]->setProperty("sampler", 0);
            materials[0]->setProperty("diffuse", Vec3(1, 1, 0));

            //1
            materials.push_back(resourceManager.createMaterial(
                "Shaders/basicDiffuse.vs",
                "Shaders/basicDiffuse.fs",
                "Shaders/wireframe.gs"));
            materials[1]->setProperty("diffuse", Vec3(0.5f, 0.0f, 0.0f));
        }

        virtual void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();

            cubes.reserve(cubesCount + 1);

            for (unsigned int i = 0; i < cubesCount; i++)
            {
                cubes.push_back(geometryFactory.createBlockMesh());
                shared_ptr<BlockMesh> cube = cubes[i];
                const float baseSize = 0.2f;
                const float scale = 0.2f + (static_cast<float>(cubesCount - i) / cubesCount) * 0.1f;
                cube->Scale(scale, scale, scale);
                cube->setMaterial(materials[0]);
                scene.add(cube.get());
            }     

            cubes.push_back(geometryFactory.createBlockMesh());
            shared_ptr<BlockMesh> cube = cubes[cubesCount];
            cube->Translate(0.5f, 0.5f, 0.5f);
            cube->Scale(5.0f, 5.0f, 5.0f);
            cube->setMaterial(materials[1]);
            scene.add(cube.get());
        }

        vector<shared_ptr<BlockMesh>> cubes;
        vector<IMaterial*> materials;
        vector<ITexture*> textures;
        const unsigned int cubesCount;
        Vec3 cubeVertices[8];
        float barycentricCoords[8];
        int interpolationSteps;
        int interpolator;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1280;
        params.height = 720;
        params.posX = 0;
        params.posY = 0;
        params.name = "Cube";
        TestWindowApp app(Vec2(static_cast<float>(params.width), 
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}