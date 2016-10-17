#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/DefaultCamera.hpp"
#include "Core/Scene.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

namespace GForceMeterDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp(const Vec2 resolution) : resolution(resolution), renderer(0) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer(resolution);
            IResourceManager& rm = renderer->getResourceManager();
            IGeometryFactory& gf = renderer->getGeometryFactory();

            intensityTexture  = rm.createRenderTarget();
            intensityTexture->addColorTexture(rm.createTexture(
                static_cast<unsigned int>(resolution.x), 
                static_cast<unsigned int>(resolution.y), 3, false));
            intensityTexture->addColorTexture(rm.createTexture(
                static_cast<unsigned int>(resolution.x),
                static_cast<unsigned int>(resolution.y), 3, false));
            intensityMaterial = rm.createMaterial("Shaders/fragmentShaderSandbox.vs", "Shaders/GForceMeter/gforceIntensity.fs");
            intensityMaterial->addTexture(intensityTexture->getColorTexture(0));

            intensityMaterial->getProperty("p", pProperty);
            intensityMaterial->getProperty("v", vProperty);

            renderMaterial = rm.createMaterial("Shaders/fragmentShaderSandbox.vs", "Shaders/GForceMeter/gforceRender.fs");
            renderMaterial->addTexture(intensityTexture->getColorTexture(0));
            rect = gf.createRectangle();
            scene.add(rect);
            time       = 0.0f;
            p0         = Vec2(0.0f);
            m0         = Vec2(0.0f);
            p1         = Vec2(0.0f);
            m1         = Vec2(0.0f);
            p          = Vec2(0.0f);
            m          = Vec2(0.0f);
            v          = Vec2(0.0f);
            prevEventV = Vec2(0.0f);
            prevEvent  = 0.0f;
            duration   = 0.2f;
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));
            
            float t = min(max((time - prevEvent) / duration, 0.0f), 1.0f);

            Vec2 oldP = p;
            p = interpolateP(p0, m0, p1, m1, t);
            m = interpolateM(p0, m0, p1, m1, t);
            v = p - oldP;
            intensityMaterial->setProperty(pProperty, p);
            intensityMaterial->setProperty(vProperty, v);
            rect->setMaterial(intensityMaterial);

            renderer->renderToTarget(scene, camera, *intensityTexture);
            rect->setMaterial(renderMaterial);

            renderer->render(scene, camera);
            time += 16.0f * 0.001f;
        }

        virtual void onDestroy() { cout << "onDestroy" << endl; }
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y) 
        { 
            cout << button << " " << state << " " << x << " " << y << endl; 
            if (!state)
            {
                p0 = p;
                m0 = m;
                
                // TODO: Aspect ratio
                Vec2 nmouseuv = (Vec2(x / resolution.x, 1.0f - y / resolution.y) - 0.5f) * 2.0f;
                p1 = nmouseuv;
                m1 = nmouseuv - p;
                prevEvent = time;
            }
        }
        virtual void onKeyboardEvent(unsigned char c, int x, int y) { }

        virtual void onMouseMove(int x, int y) { }
    private:
        template <typename T>
        void initMaterialProperty(IMaterial& material, const std::string propertyName, const T& value);
        Vec2 interpolateP(Vec2 p0, Vec2 m0, Vec2 p1, Vec2 m1, float t);
        Vec2 interpolateM(Vec2 p0, Vec2 m0, Vec2 p1, Vec2 m1, float t);

        Vec2 resolution;
        DefaultCamera camera;
        Scene scene;
        Renderer* renderer;
        IMaterial* intensityMaterial;
        IMaterial* renderMaterial;
        IRenderTarget* intensityTexture;
        float radius;
        float time;
        Vec2PropertySharedPtr  p0Property;
        Vec2PropertySharedPtr  m0Property;
        Vec2PropertySharedPtr  p1Property;
        Vec2PropertySharedPtr  m1Property;
        Vec2PropertySharedPtr  pProperty;
        Vec2PropertySharedPtr  vProperty;
        Vec2PropertySharedPtr  prevEventProperty;
        FloatPropertySharedPtr timeProperty;
        Vec2 p0;
        Vec2 m0;
        Vec2 p1;
        Vec2 m1;
        Vec2 p;
        Vec2 m;
        Vec2 v;
        Vec2 prevEventV;
        float prevEvent;
        float duration;
        shared_ptr<MeshNode> rect;
    };

    void main();
    
    template<typename T>
    inline void TestWindowApp::initMaterialProperty(IMaterial & material, const std::string propertyName, const T & value)
    {
        std::shared_ptr<IMaterialProperty<T>> p;
        material.getProperty(propertyName, p);
        if (p != 0)
        {
            material.setProperty(p, value);
        }
        else
        {
            std::cout << "Couldn't find property \"" << propertyName << "\"" << std::endl;
        }
    }
}