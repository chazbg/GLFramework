#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Core/Scene.hpp"
#include "Core/PerspectiveCamera.hpp"
#include "Demos/Demo3DBase.hpp"
#include "Math/Quarternion.hpp"
#include "Core/CameraNode.hpp"
#include "Core/Animation/LinearAnimation.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace ThirdPersonDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) :
            Demo3DBase(resolution), sphereCount(50) 
        {
        }
        ~TestWindowApp() {}

        virtual void onInit()
        {
            renderer = new Renderer(resolution);

            initTextures();
            initMaterials();
            initGeometry();

            time = 0;
            stopTime = false;
            velocity = 0.1f;
        }

        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            rollAnimation.update(0.016f);
            pitchAnimation.update(0.016f);
            accAnimation.update(0.016f);

            Vec4 jetDir = (jet->getModelMatrix() * Vec4(0.0f, 0.0f, 1.0f, 0.0f)).normalize();
            Vec3 dir = Vec3(jetDir.x, jetDir.y, jetDir.z);

            if (!rollAnimation.isExpired())
            {
                jet->rotate(rollAnimation.getValue(), dir);
            }

            if (!pitchAnimation.isExpired())
            {
                Vec4 jetRight = (jet->getModelMatrix() * Vec4(-1.0f, 0.0f, 0.0f, 0.0f)).normalize();
                Vec3 right = Vec3(jetRight.x, jetRight.y, jetRight.z);
                jet->rotate(pitchAnimation.getValue(), right);
            }

            float actualVelocity = velocity;
            if (!accAnimation.isExpired())
            {
                actualVelocity += accAnimation.getValue();
            }

            jet->translate(dir * actualVelocity);

            updateCamera();


            if (!stopTime)
            {

            }

            renderer->render(scene, camera);
        }

        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {
            cout << c << " " << x << " " << y << endl;

            Vec4 jetDir = (jet->getModelMatrix() * Vec4(0.0f, 0.0f, 1.0f, 0.0f)).normalize();
            Vec4 jetRight = (jet->getModelMatrix() * Vec4(-1.0f, 0.0f, 0.0f, 0.0f)).normalize();
            Vec3 dir = Vec3(jetDir.x, jetDir.y, jetDir.z);
            Vec3 right = Vec3(jetRight.x, jetRight.y, jetRight.z);

            switch (c)
            {
            case 'q':
            {
                accAnimation = LinearAnimation<float>();
                accAnimation.addKeyframe(AnimationKeyframe<float>(0.00f, 0.0f));
                accAnimation.addKeyframe(AnimationKeyframe<float>(0.25f, 0.3f));
                accAnimation.addKeyframe(AnimationKeyframe<float>(0.50f, 0.4f));
                accAnimation.addKeyframe(AnimationKeyframe<float>(0.75f, 0.3f));
                accAnimation.addKeyframe(AnimationKeyframe<float>(1.00f, 0.0f));
                break;
            }
            case 'a':
            {
                //Rotate around Z
                rollAnimation = LinearAnimation<float>();
                rollAnimation.addKeyframe(AnimationKeyframe<float>(0.0f, -0.03f));
                rollAnimation.addKeyframe(AnimationKeyframe<float>(0.5f, -0.03f));
                break;
            }                
            case 'd':
            {
                //Rotate around Z
                rollAnimation = LinearAnimation<float>();
                rollAnimation.addKeyframe(AnimationKeyframe<float>(0.0f, 0.03f));
                rollAnimation.addKeyframe(AnimationKeyframe<float>(0.5f, 0.03f));
                break;
            }
            case 'w':
            {
                //Rotate around X
                pitchAnimation = LinearAnimation<float>();
                pitchAnimation.addKeyframe(AnimationKeyframe<float>(0.0f, -0.03f));
                pitchAnimation.addKeyframe(AnimationKeyframe<float>(0.5f, -0.03f));
                break;
            }
            case 's':
            {
                //Rotate around X
                pitchAnimation = LinearAnimation<float>();
                pitchAnimation.addKeyframe(AnimationKeyframe<float>(0.0f, 0.03f));
                pitchAnimation.addKeyframe(AnimationKeyframe<float>(0.5f, 0.03f));
                break;
            }
            default:
                stopTime = !stopTime;
                break;
            }
        }

    private:

        void updateCamera()
        {
            Vec4 jetDir = (jet->getModelMatrix() * Vec4(0.0f, 0.0f, 1.0f, 0.0f)).normalize();
            Vec4 jetRight = (jet->getModelMatrix() * Vec4(-1.0f, 0.0f, 0.0f, 0.0f)).normalize();
            Vec3 dir = Vec3(jetDir.x, jetDir.y, jetDir.z);
            Vec3 right = Vec3(jetRight.x, jetRight.y, jetRight.z);
            float cosTheta = 0.54f;
            Quarternion rot = Quarternion::makeRotation(-cosTheta, right);
            Vec3 jetToCamera = rot.rotate(dir);
            cam->setPosition(jet->getPosition() - jetToCamera * 50.0f);
            cam->setDirVector(jet->getPosition());
            Vec3 cameraUp = right * cam->getDirVector();
            cam->setUpVector(cameraUp);
        }
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

            updateCamera();

            auto anonymous = geometryFactory.createCustomGeometry("3DAssets/StarWars/Space_Fighter.obj");
            anonymous->setMaterial(materials[0]);
            anonymous->scale(Vec3(0.01f));
            anonymous->rotate(Vec3(0.0f, static_cast<float>(M_PI), 0.0f));
            anonymous->translate(Vec3(0.0f, 0.0f, -20.0f));
            scene.add(anonymous);
        }

        std::shared_ptr<CameraNode> cam;
        std::shared_ptr<MeshNode> jet;
        std::shared_ptr<ContainerNode> container;
        vector<IMaterial*> materials;
        vector<ITexture*> textures;
        unsigned int sphereCount;
        int accState;
        int rollState;
        int pitchState;
        LinearAnimation<float> rollAnimation;
        LinearAnimation<float> pitchAnimation;
        LinearAnimation<float> accAnimation;
        float velocity;
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