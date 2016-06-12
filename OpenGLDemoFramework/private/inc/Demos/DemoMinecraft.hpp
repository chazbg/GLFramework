#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/PerspectiveCamera.hpp"
#include "Core/Scene.hpp"
#include "Demos/Demo3DBase.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

namespace MinecraftDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) : Demo3DBase(resolution) {}

        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            camera.setPosition(cameraPos);

            if (!stopTime)
            {
                time++;
            }

            renderer->setAlphaBlending(false);
            renderer->render(scene, camera);
            renderer->setAlphaBlending(true);
            renderer->render(transparentScene, camera);
        }
        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {
            cout << (int) c << " " << x << " " << y << endl;
 
            switch (c)
            {
            case 'q':
                cubes[0]->Translate(0, 1, 0);
                break;
            case 'e':
                cubes[0]->Translate(0, -1, 0);
                break;
            case 'a':
                cubes[0]->Translate(-1, 0, 0);
                break;
            case 'd':
                cubes[0]->Translate(1, 0, 0);
                break;
            case 'w':
                cubes[0]->Translate(0, 0, -1);
                break;
            case 's':
                cubes[0]->Translate(0, 0, 1);
                break;
            case 13:
            {
                IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
 
                cubes.push_back(geometryFactory.createBlockMesh());
                shared_ptr<BlockMesh> cube = cubes[cubes.size() - 1];
                cube->setMaterial(materials[1]);
                cube->setModelMatrix(cubes[0]->getModelMatrix());
 
                scene.add(cube.get());
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

            textures.push_back(rm.createTexture("Images/pattern_133/diffuse.png"));
        }

        virtual void initMaterials()
        {
            IResourceManager& resourceManager = renderer->getResourceManager();

            for (unsigned int i = 0; i < 2; i++)
            {
                materials.push_back(resourceManager.createMaterial(
                    "Shaders/minecraft.vs",
                    "Shaders/minecraft.fs"));
                initMaterialProperty(*materials[i], "sampler", 0);
                initMaterialProperty(*materials[i], "diffuse", Vec3(1.0f, 1.0f - i, 0.0f));
                initMaterialProperty(*materials[i], "alpha", 0.5f + i * 0.5f);
                materials[i]->addTexture(textures[0]);
            }
        }

        virtual void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();

            for (unsigned int i = 0; i < 1; i++)
            {
                cubes.push_back(geometryFactory.createBlockMesh());
                shared_ptr<BlockMesh> cube = cubes[i];
                cube->setMaterial(materials[i]);
                transparentScene.add(cube.get());
            }            
        }

        Scene transparentScene;
        
        vector<shared_ptr<BlockMesh>> cubes;
        vector<IMaterial*> materials;
        vector<ITexture*> textures;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1280;
        params.height = 720;
        params.posX = 0;
        params.posY = 0;
        params.name = "Minecraft";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}