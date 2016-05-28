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
    class TestWindowApp : public IApplication, public IParticle2DRenderer
    {
    public:
        TestWindowApp() : camera(), renderer(0), particleCount(100), emitter(*this, particleCount, 0.5f) {}
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
            const std::vector<IParticle2D*>& particles = emitter.getParticles();
            unsigned int aliveParticles = emitter.getAliveParticlesCount();

            for (unsigned int i = 0; i < aliveParticles; i++)
            {
                meshes[i]->getMaterial().setProperty("remainingLife", particles[i]->getRemainingLife());
            }

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

        virtual void particleSpawned(const IParticle2D& particle)
        {
            unsigned int index = emitter.getAliveParticlesCount() - 1;
            scene.add(meshes[index].get());
            meshes[index]->getMaterial().setProperty("scale", particle.getScale());
            meshes[index]->getMaterial().setProperty("duration", particle.getDuration());
            meshes[index]->getMaterial().setProperty("tangentAcceleration", particle.getTangentialAcceleration());
            meshes[index]->getMaterial().setProperty("radialAcceleration", particle.getRadialAcceleration());
        }

        virtual void particleDied(const unsigned int index)
        {
            scene.remove(meshes[index].get());
            unsigned int aliveParticles = emitter.getAliveParticlesCount();
            shared_ptr<Rectangle> tmp = meshes[index];
            meshes[index] = meshes[aliveParticles];
            meshes[aliveParticles] = tmp;
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

            materials.push_back(resourceManager.createMaterial(
                "Shaders/particle.vs",
                "Shaders/particle.fs"));
            materials[0]->setProperty("sampler", 0);
            materials[0]->addTexture(textures[0]);

            for (unsigned int i = 1; i < particleCount; i++)
            {
                materials.push_back(resourceManager.cloneMaterial(materials[0]));
            }            
        }

        void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            meshes.reserve(particleCount);
            for (unsigned int i = 0; i < particleCount; i++)
            {
                meshes.push_back(geometryFactory.createRectangle());
                meshes[i]->setMaterial(materials[i]);
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
        vector<shared_ptr<Rectangle>> meshes;
        vector<IMaterial*> materials;
        vector<ITexture*> textures;
        ITextureCubemap* envMap;
        bool cameraRotating;
        bool cameraPanning;
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