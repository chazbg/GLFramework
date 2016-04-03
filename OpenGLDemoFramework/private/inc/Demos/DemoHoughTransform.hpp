#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include <Core/DefaultCamera.hpp>
#include <Core/Scene.hpp>
#include <Core/ShaderMaterial.hpp>
#include <Core/TextureLoader.hpp>
#include <Geometry/CustomGeometry.hpp>
#include <Math/GeometryAlgorithm.hpp>
#include <Geometry/Rectangle.hpp>
#include <iostream>
#include <algorithm>

using namespace std;

namespace HoughTransformDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp() : renderer(0) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer(Vec2(1200, 600));

            initTextures();
            initMaterials();

            rect = new RectangleGeometry();
            rect->setMaterial(materials[0]);

            scene.add(rect);            
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
            materials.push_back(new ShaderMaterial("Shaders/fragmentShaderSandbox.vs", "Shaders/hough.fs"));
            materials[0]->addTexture(textures[0]);
        }

        void initTextures()
        {
            TextureLoader texLoader;

            textures.push_back(texLoader.loadTexture("Images/HoughTest/1.png"));
        }    

        DefaultCamera camera;
        Scene scene;
        Renderer* renderer;
        RectangleGeometry* rect;
        vector<IMaterial*> materials;
        vector<Texture*> textures;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1200;
        params.height = 600;
        params.posX = 0;
        params.posY = 0;
        params.name = "Hough Transform";
        TestWindowApp app;
        Window window(params, app);
        window.startRenderLoop();
    }
}