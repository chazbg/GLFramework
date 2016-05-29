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

            prevDir += Vec3(0, -0.001f, 0);
            meshes[4]->Translate(prevDir.x, prevDir.y, prevDir.z);

            shared_ptr<BlockMesh> animatedMeshes[3] = { c1, meshes[0], meshes[1] };

            for (int i = 0; i < 3; i++)
            {
                Vec3 pos = animatedMeshes[i]->getPosition();
                animatedMeshes[i]->Translate(-pos.x, -pos.y, -pos.z);
                animatedMeshes[i]->Rotate(0, 0.01f, 0);
                animatedMeshes[i]->Translate(pos.x, pos.y, pos.z);
            }

            cubeTexturedMat->setProperty("time", (float) time);
            cubeTexturedMat2->setProperty("time", time * 0.1f);

            renderer->render(scene, camera);
        }

        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {
            Demo3DBase::onKeyboardEvent(c, x, y);

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
                meshes[4]->Translate(-pos.x, -pos.y, -pos.z);
                meshes[4]->Translate(-10.0f, 3.0f, 0.0f);
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
            cubeTexturedMat = resourceManager.createMaterial("Shaders/texturedCube.vs", "Shaders/texturedCube.fs");
            cubeTexturedMat->addTexture(textures[0]);
            cubeTexturedMat->setProperty("colorMap", 0);
            cubeTexturedMat->setProperty("sampler", 1);
            cubeTexturedMat->setProperty("diffuse", Vec3(0.2f, 0.2f, 0.8f));

            cubeTexturedMat2 = resourceManager.cloneMaterial(cubeTexturedMat);
            cubeTexturedMat2->addTexture(textures[1]);
            cubeTexturedMat2->setProperty("diffuse", Vec3(0.3f, 0, 0));
        }

        virtual void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();

            c1 = geometryFactory.createBlockMesh();
            c1->Scale(2.0f, 5.0f, 5.0f);
            c1->Translate(0, -1, 0);
            c1->setMaterial(cubeMat);
            meshes.push_back(geometryFactory.createBlockMesh());
            meshes[0]->Scale(2.0f, 5.0f, 5.0f);
            meshes[0]->Translate(-2, -1, 6);
            meshes[0]->setMaterial(cubeMat);
            meshes.push_back(geometryFactory.createBlockMesh());
            meshes[1]->Scale(2.0f, 5.0f, 5.0f);
            meshes[1]->Translate(2, -1, -6);
            meshes[1]->setMaterial(cubeMat);
            meshes.push_back(geometryFactory.createBlockMesh());
            meshes[2]->Scale(20.0f, 0.2f, 20.0f);
            meshes[2]->Translate(0, -3, 0);
            meshes[2]->setMaterial(cubeTexturedMat2);
            meshes.push_back(geometryFactory.createBlockMesh());
            meshes[3]->Scale(20.0f, 20.0f, 0.2f);
            meshes[3]->Translate(0, -3, -10);
            meshes[3]->setMaterial(cubeTexturedMat);
            meshes.push_back(geometryFactory.createBlockMesh());
            meshes[4]->Scale(0.5f, 0.5f, 0.5f);
            meshes[4]->setMaterial(cubeMat);

            scene.add(c1.get());
            scene.add(meshes[0].get());
            scene.add(meshes[1].get());
            scene.add(meshes[2].get());
            scene.add(meshes[3].get());
            scene.add(meshes[4].get());
        }

        shared_ptr<BlockMesh> c1;
        vector<shared_ptr<BlockMesh>> meshes;
        IMaterial* cubeMat;
        IMaterial* cubeTexturedMat;
        IMaterial* cubeTexturedMat2;
        vector<ITexture*> textures;
        Vec3 prevDir;
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