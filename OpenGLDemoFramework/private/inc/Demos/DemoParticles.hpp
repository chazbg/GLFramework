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
        TestWindowApp(const Vec2& resolution) : 
            camera(), 
            resolution(resolution), 
            renderer(0), 
            particleCount(100), 
            emitter(*this, particleCount, 0.5f) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer(resolution);

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
                meshes[i]->getMaterial().setProperty(remainingLife[i], particles[i]->getRemainingLife());
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
            meshes[index]->getMaterial().setProperty(scale[index], particle.getScale());
            meshes[index]->getMaterial().setProperty(duration[index], particle.getDuration());
            meshes[index]->getMaterial().setProperty(tangentAcceleration[index], particle.getTangentialAcceleration());
            meshes[index]->getMaterial().setProperty(radialAcceleration[index], particle.getRadialAcceleration());
            meshes[index]->getMaterial().setProperty(phase[index], particle.getPhase());
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
            shared_ptr<IMaterialProperty<int>> samplerProperty;
            materials[0]->getProperty("sampler", samplerProperty);
            materials[0]->setProperty(samplerProperty, 0);
            materials[0]->addTexture(textures[0]);

            for (unsigned int i = 1; i < particleCount; ++i)
            {
                materials.push_back(resourceManager.cloneMaterial(materials[0]));
            }    

            for (unsigned int i = 0; i < particleCount; ++i)
            {
                shared_ptr<IMaterialProperty<float>> p;
                shared_ptr<IMaterialProperty<Vec2>> p2;
                materials[i]->getProperty("remainingLife", p);
                remainingLife.push_back(p);
                materials[i]->getProperty("duration", p);
                duration.push_back(p);
                materials[i]->getProperty("tangentAcceleration", p);
                tangentAcceleration.push_back(p);
                materials[i]->getProperty("radialAcceleration", p);
                radialAcceleration.push_back(p);
                materials[i]->getProperty("scale", p2);
                scale.push_back(p2);
                materials[i]->getProperty("phase", p);
                phase.push_back(p);
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
        Vec2 resolution;
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
        vector<shared_ptr<IMaterialProperty<float>>> remainingLife;
        vector<shared_ptr<IMaterialProperty<float>>> duration;
        vector<shared_ptr<IMaterialProperty<float>>> tangentAcceleration;
        vector<shared_ptr<IMaterialProperty<float>>> radialAcceleration;
        vector<shared_ptr<IMaterialProperty<Vec2>>>  scale;
        vector<shared_ptr<IMaterialProperty<float>>> phase;
    };

    void main()
    {
        WindowParameters params;
        params.width = 600;
        params.height = 600;
        params.posX = 0;
        params.posY = 0;
        params.name = "Particles";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}