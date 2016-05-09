#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/PerspectiveCamera.hpp"
#include "Core/Scene.hpp"
#include "Geometry/CustomGeometry.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

namespace GeometryShaderDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp() : camera(3.0f / 4.0f, 16.0f / 9.0f, 1.0f, 1000.0f), renderer(0) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer(Vec2(1920, 1080));

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

            renderer->render(scene, camera);
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
            cout << c << " " << x << " " << y << endl;

            switch (c)
            {
            default:
                stopTime = !stopTime;
                break;
            }
        }

        virtual void onMouseMove(int x, int y)
        {
            Vec2 delta = Vec2(static_cast<float>(x), static_cast<float>(y)) - prevMousePos;
            delta.x /= 1920.0f;
            delta.y /= 1080.0f;
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

        void initMaterials()
        {
            IResourceManager& resourceManager = renderer->getResourceManager();

            //0
            materials.push_back(resourceManager.createMaterial(
                "Shaders/basicDiffuse.vs",
                "Shaders/basicDiffuse.fs"));
            materials[0]->setProperty("diffuse", Vec3(1.0f, 1.0f, 0.0f));

            //1
            materials.push_back(resourceManager.createMaterial(
                "Shaders/basicDiffuse.vs", 
                "Shaders/basicDiffuse.fs", 
                "Shaders/basicGeometry.gs"));
            materials[1]->setProperty("diffuse", Vec3(1.0f, 0.0f, 0.0f));
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

        void updateCamera()
        {
            float t = radius * cos(theta);
            cameraPos = camera.getLookDirection() + Vec3(t * cos(phi), radius * sin(theta), t * sin(phi));
        }

        void showNextMesh()
        {
            meshIndex = (meshIndex + 1) % 4;
            scene.remove(currentMesh);
            currentMesh = meshes[meshIndex];
            currentMesh->setMaterial(materials[11 + materialIndex]);
            scene.add(currentMesh);
        }

        void showPreviousMesh()
        {
            meshIndex = (0 == meshIndex ? 3 : (meshIndex - 1) % 4);
            scene.remove(currentMesh);
            currentMesh = meshes[meshIndex];
            currentMesh->setMaterial(materials[11 + materialIndex]);
            scene.add(currentMesh);
        }

        void showNextMaterial()
        {
            materialIndex = (materialIndex + 1) % 4;
            currentMesh->setMaterial(materials[11 + materialIndex]);
        }

        void showPreviousMaterial()
        {
            materialIndex = (0 == materialIndex ? 3 : (materialIndex - 1) % 4);
            currentMesh->setMaterial(materials[11 + materialIndex]);
        }

        PerspectiveCamera camera;
        Scene scene;
        Renderer* renderer;
        CustomGeometry* currentMesh;
        CustomGeometry* environmentCube;
        CustomGeometry* ground[9];
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
        vector<CustomGeometry*> lights;
        vector<CustomGeometry*> meshes;
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
        params.width = 1920;
        params.height = 1080;
        params.posX = 0;
        params.posY = 0;
        params.name = "PhysicallyBasedShading";
        TestWindowApp app;
        Window window(params, app);
        window.startRenderLoop();
    }
}