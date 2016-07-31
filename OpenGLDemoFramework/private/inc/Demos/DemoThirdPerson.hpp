#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Core/Scene.hpp"
#include "Core/PerspectiveCamera.hpp"
#include "Demos/Demo3DBase.hpp"
#include "Math/Quarternion.hpp"
#include "Core/CameraNode.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace ThirdPersonDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) :
            Demo3DBase(resolution), sphereCount(50) {}
        ~TestWindowApp() {}

        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            Quarternion rotA = Quarternion::makeRotation(static_cast<float>(M_PI) / 30.0f, Vec3(-1.0f, 1.0f, 0.0f));


            if (!stopTime)
            {

            }

            renderer->render(scene, camera);
        }

        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {
            cout << c << " " << x << " " << y << endl;

            switch (c)
            {
            case 'q':
            {
                cam->translate(-camera.getLookDirection().normalize());
                jet->translate(-camera.getLookDirection().normalize());
                break;
            }
            case 'e':
            {
                cam->translate(camera.getLookDirection().normalize());
                jet->translate(camera.getLookDirection().normalize());
                break;
            }
            case 'a':
            {
                //Rotate around Z
                Vec3 dir = camera.getLookDirection();
                cam->rotate(-0.1f, dir);
                jet->rotate(-0.1f, dir);
                break;
            }                
            case 'd':
            {
                //Rotate around Z
                Vec3 dir = camera.getLookDirection();
                cam->rotate(0.1f, dir);
                jet->rotate(0.1f, dir);
                break;
            }
            case 'w':
            {
                //Rotate around X
                Vec3 dir = (camera.getLookDirection() * camera.getUpVector()).normalize();
                cam->rotate(-0.1f, dir);
                jet->rotate(-0.1f, dir);
                break;
            }
            case 's':
            {
                //Rotate around X
                Vec3 dir = (camera.getLookDirection() * camera.getUpVector()).normalize();
                cam->rotate(0.1f, dir);
                jet->rotate(0.1f, dir);
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

            jet = geometryFactory.createCustomGeometry("3DAssets/StarWars/X-Fighter.obj");
            jet->setMaterial(materials[0]);
            jet->scale(Vec3(0.01f));
            jet->rotate(Vec3(0.0f, static_cast<float>(M_PI), 0.0f));

            cam = std::shared_ptr<CameraNode>(new CameraNode(camera));

            scene.add(jet);
            scene.add(cam);

            auto anonymous = geometryFactory.createCustomGeometry("3DAssets/StarWars/Space_Fighter.obj");
            anonymous->setMaterial(materials[0]);
            anonymous->scale(Vec3(0.01f));
            anonymous->rotate(Vec3(0.0f, static_cast<float>(M_PI), 0.0f));
            anonymous->translate(Vec3(0.0f, 0.0f, -20.0f));
            scene.add(anonymous);
        }
      
        void update()
        {
            if (accState == -1)
            {
                cam->translate(-camera.getLookDirection().normalize());
                jet->translate(-camera.getLookDirection().normalize());
            }

            if (accState == 1)
            {
                cam->translate(camera.getLookDirection().normalize());
                jet->translate(camera.getLookDirection().normalize());
            }

            if (rollState == -1)
            {
                //Rotate around Z
                Vec3 dir = camera.getLookDirection();
                cam->rotate(Vec3(0.0f, 0.0f, 0.1f));
                jet->rotate(Vec3(0.0f, 0.0f, -0.1f));
            }

            if (rollState == 1)
            {
                //Rotate around Z
                Vec3 dir = camera.getLookDirection();
                cam->rotate(Vec3(0.0f, 0.0f, -0.1f));
                jet->rotate(Vec3(0.0f, 0.0f, 0.1f));
            }

            if (pitchState == -1)
            {
                //Rotate around X
                Vec3 dir = (camera.getLookDirection() * camera.getUpVector()).normalize();
                cam->rotate(-0.1f, dir);
                jet->rotate(Vec3(0.1f, 0.0f, 0.0f));
            }

            if (pitchState == 1)
            {
                //Rotate around X
                Vec3 dir = (camera.getLookDirection() * camera.getUpVector()).normalize();
                cam->rotate(0.1f, dir);
                jet->rotate(Vec3(-0.1f, 0.0f, 0.0f));
            }
        }

        std::shared_ptr<CameraNode> cam;
        std::shared_ptr<MeshNode> jet;
        vector<IMaterial*> materials;
        vector<ITexture*> textures;
        unsigned int sphereCount;
        int accState;
        int rollState;
        int pitchState;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1280;
        params.height = 720;
        params.posX = 0;
        params.posY = 0;
        params.name = "ThirdPerson";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
            static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}