#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Core/Scene.hpp"
#include "Core/PerspectiveCamera.hpp"
#include "Demos/Demo3DBase.hpp"
#include "Math/Quarternion.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace QuarternionDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) :
            Demo3DBase(resolution), sphereCount(50) {}
        ~TestWindowApp() {}

        virtual void onRender(const unsigned int deltaTime)
        {
            Demo3DBase::onRender(deltaTime);

            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            Quarternion rotA = Quarternion::makeRotation(static_cast<float>(M_PI) / 30.0f, Vec3(-1.0f, 1.0f, 0.0f));
            Quarternion rotB = Quarternion::makeRotation(static_cast<float>(M_PI) / 30.0f, Vec3(1.0f, 1.0f, 1.0f));
            Quarternion rotC = Quarternion::makeRotation(static_cast<float>(M_PI) / 30.0f, Vec3(1.0f, 0.0f, 0.0f));

            if (!stopTime)
            {
                Quarternion rot = Quarternion::slerp(rotA, rotB, abs(sin(time * 0.02f)));
                updateSpheres(spheres, rot);

                spheres2[0]->Translate(sin(time * 0.02f) * 0.1f, 0.0f, 0.0f);

                updateSpheres(spheres2, rotC);               
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
                "Shaders/cube.vs",
                "Shaders/cube.fs"));
        }

        virtual void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();

            for (unsigned int i = 0; i < sphereCount; i++)
            {
                spheres.push_back(geometryFactory.createCustomGeometry("3DAssets/Sphere.3ds"));

                spheres[i]->setMaterial(materials[0]);
                float scale = 0.05f + 0.05f * (sphereCount - i) / static_cast<float>(sphereCount);
                spheres[i]->Scale(scale, scale, scale);
                spheres[i]->Translate(2.0f, 0.0f, 0.0f);
                scene.add(spheres[i].get());
            }   

            for (unsigned int i = 0; i < sphereCount; i++)
            {
                spheres2.push_back(geometryFactory.createCustomGeometry("3DAssets/Sphere.3ds"));

                spheres2[i]->setMaterial(materials[0]);
                float scale = 0.05f + 0.05f * (sphereCount - i) / static_cast<float>(sphereCount);
                spheres2[i]->Scale(scale, scale, scale);
                spheres2[i]->Translate(-4.0f, 4.0f, 0.0f);
                scene.add(spheres2[i].get());
            }
        }

        void updateSpheres(vector <shared_ptr<CustomGeometry>>& spheres, const Quarternion& rotation)
        {
            Vec3 newPos = rotation.rotate(spheres[0]->getPosition());

            for (unsigned int i = spheres.size() - 1; i >= 1; i--)
            {
                Vec3 translation = spheres[i - 1]->getPosition() - spheres[i]->getPosition();
                auto sphere = spheres[i];
                sphere->Translate(translation.x, translation.y, translation.z);
            }

            Vec3 translation = newPos - spheres[0]->getPosition();
            spheres[0]->Translate(translation.x, translation.y, translation.z);
        }
        vector<shared_ptr<CustomGeometry>> spheres;
        vector<shared_ptr<CustomGeometry>> spheres2;
        vector<IMaterial*> materials;
        vector<ITexture*> textures;
        unsigned int sphereCount;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1280;
        params.height = 720;
        params.posX = 0;
        params.posY = 0;
        params.name = "Quarternion";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
            static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}