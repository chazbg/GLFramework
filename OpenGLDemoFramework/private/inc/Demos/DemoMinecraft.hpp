#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/PerspectiveCamera.hpp"
#include "Core/Scene.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

namespace MinecraftDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp() : camera(3.0f / 4.0f, 16.0f / 9.0f, 1.0f, 1000.0f), renderer(0) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer(Vec2(1280, 720));

            initTextures();
            initMaterials();
            initGeometry();

            time = 0;
            stopTime = false;
            prevDir = Vec3(0.01f, 0.1f, 0);
            prevMousePos = Vec2(0.5, 0.5);
            phi = 3.14f / 2.0f;
            theta = 0;
            radius = 30;
            float t = radius * cos(theta);
            cameraPos = Vec3(t * cos(phi), radius * sin(theta), t * sin(phi));
            ior = 0.01f;
            glossiness = 0.01f;
            materialIndex = 0;
            meshIndex = 0;
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
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

        virtual void onDestroy() {}
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y)
        {
            cout << "onMouseEvent: " << button << " " << state << endl;
            if (0 == button)
            {
                cameraRotating = (0 == state ? true : false);
            }

            if (2 == button)
            {
                cameraPanning = (0 == state ? true : false);
            }

            if (3 == button)
            {
                radius -= 1.0f;
                updateCamera();
            }

            if (4 == button)
            {
                radius += 1.0f;
                updateCamera();
            }
        }

        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {
            cout << (int) c << " " << x << " " << y << endl;

            switch (c)
            {
            case 'q':
                cubes[0].Translate(0, 1, 0);
                break;
            case 'e':
                cubes[0].Translate(0, -1, 0);
                break;
            case 'a':
                cubes[0].Translate(-1, 0, 0);
                break;
            case 'd':
                cubes[0].Translate(1, 0, 0);
                break;
            case 'w':
                cubes[0].Translate(0, 0, -1);
                break;
            case 's':
                cubes[0].Translate(0, 0, 1);
                break;
            case 13:
            {
                IGeometryFactory& geometryFactory = renderer->getGeometryFactory();

                cubes.push_back(geometryFactory.createBlockMesh());
                BlockMesh* cube = &(*(cubes.begin() + cubes.size() - 1));
                cube->setMaterial(materials[1]);
                cube->setModelMatrix(cubes[0].getModelMatrix());

                vector<IMesh*>& children = scene.getChildren();
                children.clear();

                vector<IMesh*>& children2 = transparentScene.getChildren();
                children2.clear();

                for (unsigned int i = 0; i < cubes.size(); i++)
                {
                    if (i == 0)
                    {
                        transparentScene.add(&(*(cubes.begin() + i)));
                    }
                    else
                    {
                        scene.add(&(*(cubes.begin() + i)));
                    }
                }
                break;
            }
            default:
                stopTime = !stopTime;
                break;
            }
        }

        virtual void onMouseMove(int x, int y)
        {
            Vec2 delta = Vec2(static_cast<float>(x), static_cast<float>(y)) - prevMousePos;
            delta.x /= 1280.0f;
            delta.y /= 720.0f;
            if (cameraRotating)
            {
                phi += 3.14f * delta.x;
                theta += 3.14f * delta.y;
                updateCamera();
            }

            if (cameraPanning)
            {
                Vec3 center = camera.getLookDirection();
                Vec3 z = center - cameraPos;
                Vec3 y = camera.getUpVector();
                Vec3 dx = (z * y).normalize();
                Vec3 dy = (z * dx).normalize();

                cameraPos += (dx * delta.x + dy * delta.y) * 30.0f;
                camera.setLookDirection(center + (dx * delta.x + dy * delta.y) * 30.0f);
            }

            prevMousePos = Vec2(static_cast<float>(x), static_cast<float>(y));
        }
    private:

        void initTextures()
        {
            IResourceManager& rm = renderer->getResourceManager();

            textures.push_back(rm.createTexture("Images/pattern_133/diffuse.png"));
        }

        void initMaterials()
        {
            IResourceManager& resourceManager = renderer->getResourceManager();

            for (unsigned int i = 0; i < 2; i++)
            {
                materials.push_back(resourceManager.createMaterial(
                    "Shaders/minecraft.vs",
                    "Shaders/minecraft.fs"));
                materials[i]->setProperty("sampler", 0);
                materials[i]->setProperty("diffuse", Vec3(1, 1 - i, 0));
                materials[i]->setProperty("alpha", 0.5f + i * 0.5f);
                materials[i]->addTexture(textures[0]);
            }
        }

        void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();

            for (unsigned int i = 0; i < 1; i++)
            {
                cubes.push_back(geometryFactory.createBlockMesh());
                BlockMesh* cube = &(*(cubes.begin() + i));
                cube->setMaterial(materials[i]);
                transparentScene.add(cube);
            }            
        }

        void updateCamera()
        {
            float t = radius * cos(theta);
            cameraPos = camera.getLookDirection() + Vec3(t * cos(phi), radius * sin(theta), t * sin(phi));
        }

        PerspectiveCamera camera;
        Scene scene;
        Scene transparentScene;
        Renderer* renderer;
        unsigned int time;
        bool stopTime;
        Vec3 cameraPos;
        Vec3 prevDir;
        Vec2 prevMousePos;
        float phi;
        float theta;
        float radius;
        float ior;
        float glossiness;
        vector<BlockMesh> cubes;
        vector<IMaterial*> materials;
        vector<ITexture*> textures;
        ITextureCubemap* envMap;
        bool cameraRotating;
        bool cameraPanning;
        int materialIndex;
        int meshIndex;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1280;
        params.height = 720;
        params.posX = 0;
        params.posY = 0;
        params.name = "Minecraft";
        TestWindowApp app;
        Window window(params, app);
        window.startRenderLoop();
    }
}