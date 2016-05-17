#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/DefaultCamera.hpp"
#include "Core/Scene.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/Particles/SimpleEmitter.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

namespace ParticlesDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp() : camera(), renderer(0), particleCount(100), emitter(particleCount, 1.0f) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer(Vec2(600, 600));

            renderer->setAlphaBlending(true);
            renderer->setDepthTest(false);

            initTextures();
            initMaterials();
            initGeometry();

            time = 0;
            stopTime = false;
            prevDir = Vec3(0.01f, 0.1f, 0);
            prevMousePos = Vec2(0.5, 0.5);
            
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

            emitter.updateParticles(0.03f);
            const std::vector<SimpleParticle*>& particles = emitter.getParticles();
            unsigned int aliveParticles = emitter.getAliveParticlesCount();

            for (unsigned int i = 0; i < aliveParticles; i++)
            {
                const SimpleParticle* particle = particles[i];

                Matrix4 mat(
                    Vec4(0.1, 0, 0, particle->particlePos.x),
                    Vec4(0, 0.1, 0, particle->particlePos.y),
                    Vec4(0, 0, 1, 0),
                    Vec4(0, 0, 0, 1));

                meshes[i]->setModelMatrix(mat);
                meshes[i]->getMaterial().setProperty("alpha", particle->alpha);
            }

            for (unsigned int i = aliveParticles; i < particleCount - aliveParticles; i++)
            {
                meshes[i]->getMaterial().setProperty("alpha", 0.0f);
            }
            
			//meshes[0]->Rotate(0, 0.1, 0);

            renderer->render(scene, camera);
        }

        virtual void onDestroy() {}
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y)
        {
            cout << "onMouseEvent: " << button << " " << state << endl;
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
            prevMousePos = Vec2(static_cast<float>(x), static_cast<float>(y));
        }
    private:

        void initTextures()
        {
            IResourceManager& rm = renderer->getResourceManager();
   
            textures.push_back(rm.createTexture("Images/particletexture.jpg"));
        }

        void initMaterials()
        {
            IResourceManager& resourceManager = renderer->getResourceManager();

            for (unsigned int i = 0; i < particleCount; i++)
            {
                materials.push_back(resourceManager.createMaterial(
                    "Shaders/particle.vs",
                    "Shaders/particle.fs"));
                materials[i]->setProperty("sampler", 0);
                materials[i]->addTexture(textures[0]);
            }            
        }

        void initGeometry()
        {
            IResourceManager& rm = renderer->getResourceManager();
            for (unsigned int i = 0; i < particleCount; i++)
            {
                meshes.push_back(new Rectangle(rm));
                meshes[i]->setMaterial(materials[i]);
                scene.add(meshes[i]);
            }
        }


        DefaultCamera camera;
        Scene scene;
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
        vector<IMesh*> meshes;
        vector<IMaterial*> materials;
        vector<ITexture*> textures;
        ITextureCubemap* envMap;
        bool cameraRotating;
        bool cameraPanning;
        int materialIndex;
        int meshIndex;
        unsigned int particleCount;
        SimpleEmitter emitter;
    };

    void main()
    {
        WindowParameters params;
        params.width = 600;
        params.height = 600;
        params.posX = 0;
        params.posY = 0;
        params.name = "Particles";
        TestWindowApp app;
        Window window(params, app);
        window.startRenderLoop();
    }
}