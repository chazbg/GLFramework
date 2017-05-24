#pragma once
#include "Core/TextureGenerator.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Demos/Demo3DBase.hpp"
#include <vector>

using namespace std;

namespace TextureDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) : Demo3DBase(resolution) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            Demo3DBase::onInit();

            prevDir = Vec3(0.01f, 0.1f, 0);
        }

        virtual void onRender(const unsigned int deltaTime)
        {
            Demo3DBase::onRender(deltaTime);

            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            if (!stopTime)
            {
                prevDir += Vec3(0, -0.001f, 0);
                meshes[4]->translate(prevDir);

                MeshNodeSharedPtr animatedMeshes[3] = { c1, meshes[0], meshes[1] };

                for (int i = 0; i < 3; i++)
                {
                    Vec3 pos = animatedMeshes[i]->getPosition();
                    animatedMeshes[i]->translate(-pos);
                    animatedMeshes[i]->rotate(Vec3(0.0f, 0.01f, 0.0f));
                    animatedMeshes[i]->translate(pos);
                }

                cubeTexturedMat[0]->setProperty(timeProperty[0], static_cast<float>(time));
                cubeTexturedMat[1]->setProperty(timeProperty[1], time * 0.1f);
            }

            //renderer->renderToTarget(scene, camera, )
            renderer->render(scene, camera);
        }

        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {
            switch (c)
            {
            case 'j':
                time -= 5;
                break;
            case 'l':
                time += 5;
                break;
            case 'r':
            {
                prevDir = Vec3(0.01f, 0.1f, 0.0f);
                Vec3 pos = meshes[4]->getPosition();
                meshes[4]->translate(-pos);
                meshes[4]->translate(Vec3(-10.0f, 3.0f, 0.0f));
                break;
            }
            default:
                stopTime = !stopTime;
                break;
            }
        }
    private:
        virtual void initTextures()
        {
            IResourceManager& rm = renderer->getResourceManager();
            TextureGenerator gen;

            textures.push_back(rm.createTexture(256, 256, 4,
                (unsigned char*)gen.generateOctavePerlinNoise(1.0f, 0.0f, 3, 0.5f)));
            textures.push_back(rm.createTexture(256, 256, 4,
                (unsigned char*)gen.generateOctavePerlinNoise(1.0f, 0.0f, 4, 0.7f)));
        }

        virtual void initMaterials()
        {
            IResourceManager& resourceManager = renderer->getResourceManager();

            cubeMat = resourceManager.createMaterial("Shaders/cube.vs", "Shaders/cube.fs");
            cubeTexturedMat[0] = resourceManager.createMaterial("Shaders/texturedCube.vs", "Shaders/texturedCube.fs");
            cubeTexturedMat[1] = resourceManager.cloneMaterial(cubeTexturedMat[0]);

            cubeTexturedMat[0]->addTexture(textures[0]);
            initMaterialProperty(*cubeTexturedMat[0], "colorMap", 0);
            initMaterialProperty(*cubeTexturedMat[0], "sampler", 1);
            initMaterialProperty(*cubeTexturedMat[0], "diffuse", Vec3(0.2f, 0.2f, 0.8f));
            cubeTexturedMat[0]->getProperty("time", timeProperty[0]);

            cubeTexturedMat[1]->addTexture(textures[1]);
            initMaterialProperty(*cubeTexturedMat[0], "colorMap", 0);
            initMaterialProperty(*cubeTexturedMat[1], "sampler", 1);
            initMaterialProperty(*cubeTexturedMat[1], "diffuse", Vec3(0.8f, 0.2f, 0.0f));
            cubeTexturedMat[1]->getProperty("time", timeProperty[1]);
        }

        virtual void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();

            c1 = geometryFactory.createBlockMesh();
            c1->scale(Vec3(2.0f, 5.0f, 5.0f));
            c1->translate(Vec3(0.0f, -1.0f, 0.0f));
            c1->setMaterial(cubeMat);
            meshes.push_back(geometryFactory.createBlockMesh());
            meshes[0]->scale(Vec3(2.0f, 5.0f, 5.0f));
            meshes[0]->translate(Vec3(-2.0f, -1.0f, 6.0f));
            meshes[0]->setMaterial(cubeMat);
            meshes.push_back(geometryFactory.createBlockMesh());
            meshes[1]->scale(Vec3(2.0f, 5.0f, 5.0f));
            meshes[1]->translate(Vec3(2.0f, -1.0f, -6.0f));
            meshes[1]->setMaterial(cubeMat);
            meshes.push_back(geometryFactory.createBlockMesh());
            meshes[2]->scale(Vec3(20.0f, 0.2f, 20.0f));
            meshes[2]->translate(Vec3(0.0f, -3.0f, 0.0f));
            meshes[2]->setMaterial(cubeTexturedMat[1]);
            meshes.push_back(geometryFactory.createBlockMesh());
            meshes[3]->scale(Vec3(20.0f, 20.0f, 0.2f));
            meshes[3]->translate(Vec3(0.0f, -3.0f, -10.0f));
            meshes[3]->setMaterial(cubeTexturedMat[0]);
            meshes.push_back(geometryFactory.createBlockMesh());
            meshes[4]->scale(Vec3(0.5f));
            meshes[4]->setMaterial(cubeMat);

            scene.add(c1);
            scene.add(meshes[0]);
            scene.add(meshes[1]);
            scene.add(meshes[2]);
            scene.add(meshes[3]);
            scene.add(meshes[4]);
        }

        MeshNodeSharedPtr c1;
        vector<MeshNodeSharedPtr> meshes;
        IMaterial* cubeMat;
        IMaterial* cubeTexturedMat[2];
        vector<ITexture*> textures;
        Vec3 prevDir;
        FloatPropertySharedPtr timeProperty[2];
    };

    void main()
    {
        WindowParameters params;
        params.width = 1024;
        params.height = 1024;
        params.posX = 100;
        params.posY = 100;
        params.name = "TestWindow";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}