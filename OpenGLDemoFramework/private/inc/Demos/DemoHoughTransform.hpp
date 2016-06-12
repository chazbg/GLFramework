#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/DefaultCamera.hpp"
#include "Core/Scene.hpp"
#include "Geometry/Rectangle.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

namespace HoughTransformDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp(const Vec2 resolution) : renderer(0) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer(resolution);

            initTextures();
            initMaterials();

            IGeometryFactory& gf = renderer->getGeometryFactory();
            rect = gf.createRectangle();
            rect->setMaterial(materials[0]);

            scene.add(rect.get());            
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            renderer->render(scene, camera);
        }

        virtual void onDestroy() {}
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y)
        {

        }

        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {

        }

        virtual void onMouseMove(int x, int y)
        {
            
        }
    private:

        void initMaterials()
        {
            IResourceManager& rm = renderer->getResourceManager();
            materials.push_back(rm.createMaterial("Shaders/fragmentShaderSandbox.vs", "Shaders/hough.fs"));
            materials[0]->addTexture(textures[0]);
        }

        void initTextures()
        {
            IResourceManager& rm = renderer->getResourceManager();
            textures.push_back(rm.createTexture("Images/HoughTest/1.png"));
        }    

        Vec2 resolution;
        DefaultCamera camera;
        Scene scene;
        Renderer* renderer;
        std::shared_ptr<Rectangle> rect;
        vector<IMaterial*> materials;
        vector<ITexture*> textures;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1200;
        params.height = 600;
        params.posX = 0;
        params.posY = 0;
        params.name = "Hough Transform";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}