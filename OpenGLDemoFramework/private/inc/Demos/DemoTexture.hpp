#pragma once
#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/TextureGenerator.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Core/FrameBuffer.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Geometry/Triangle.hpp"
#include <GL/glew.h>
#include <vector>
#include "Core/PerspectiveCamera.hpp"
#include "Core/Scene.hpp"

using namespace std;

namespace TexDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp() : camera(3.0f / 4.0f, 16.0f / 9.0f, 1.0f, 1000.0f), renderer(0) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer(Vec2(1920, 1080));

            TextureFactory texFactory;
            TextureGenerator gen;

            time = 0;
            stopTime = false;
            cameraPos = Vec3(0, 15, 25);
            meshPos = Vec3(-10.0, 3, 0);
            prevDir = Vec3(0.01f, 0.1f, 0);
            cubeMat = new OpenGLMaterial("Shaders/cube.vs", "Shaders/cube.fs");
            cubeTexturedMat = new OpenGLMaterial("Shaders/texturedCube.vs", "Shaders/texturedCube.fs");
            cubeTexturedMat->addTexture(texFactory.createTexture(256, 256, 4, (unsigned char*)gen.generateOctavePerlinNoise(1, 0, 3, 0.5)));
            cubeTexturedMat->setProperty("colorMap", 0);
            cubeTexturedMat->setProperty("sampler", 1);
            cubeTexturedMat->setProperty("diffuse", Vec3(0, 0, 0.8f));

            cubeTexturedMat2 = new OpenGLMaterial("Shaders/texturedCube.vs", "Shaders/texturedCube.fs");
            cubeTexturedMat2->addTexture(texFactory.createTexture(256, 256, 4, (unsigned char*)gen.generateOctavePerlinNoise(1, 0, 4, 0.7)));
            cubeTexturedMat2->setProperty("colorMap", 0);
            cubeTexturedMat2->setProperty("sampler", 1);
            cubeTexturedMat2->setProperty("diffuse", Vec3(0.3f, 0, 0));

            c1 = new BlockMesh(2.0f, 5.0f, 5.0f);
            c1->SetPosition(Vec3(0, -1, 0));
            c1->setMaterial(cubeMat);
            meshes.push_back(new BlockMesh(2.0f, 5.0f, 5.0f));
            meshes[0]->SetPosition(Vec3(-2, -1, -6));
            meshes[0]->setMaterial(cubeMat);
            meshes.push_back(new BlockMesh(2.0f, 5.0f, 5.0f));
            meshes[1]->SetPosition(Vec3(2, -1, 6));
            meshes[1]->setMaterial(cubeMat);
            meshes.push_back(new BlockMesh(20.0f, 0.2f, 20.0f));
            meshes[2]->SetPosition(Vec3(0, -3, 0));
            meshes[2]->setMaterial(cubeTexturedMat2);
            meshes.push_back(new BlockMesh(20.0f, 20.0f, 0.2f));
            meshes[3]->SetPosition(Vec3(0, -3, -10));
            meshes[3]->setMaterial(cubeTexturedMat);
            meshes.push_back(new BlockMesh(0.5f, 0.5f, 0.5f));
            meshes[4]->setMaterial(cubeMat);
            //meshes[4]->SetPosition(Vec3(0, 10, 0));

            scene.add(c1);
            //scene.add(r);
            scene.add(meshes[0]);
            scene.add(meshes[1]);
            scene.add(meshes[2]);
            scene.add(meshes[3]);
            scene.add(meshes[4]);

            prevMousePos = Vec2(0.5, 0.5);
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            camera.setPosition(cameraPos);

            prevDir += Vec3(0, -0.001f, 0);
            meshPos += prevDir;
            meshes[4]->SetPosition(meshPos);

            c1->SetRotation(0, time * 0.01f, 0);
            meshes[0]->SetRotation(0, time * 0.01f, 0);
            meshes[1]->SetRotation(0, time * 0.01f, 0);
            cubeTexturedMat->setProperty("time", (float) time);
            cubeTexturedMat2->setProperty("time", time * 0.1f);

            renderer->render(scene, camera);
            if (!stopTime)
            {
                time++;
            }
        }

        virtual void onDestroy()
        {
            delete renderer;
            delete c1;
            for (unsigned int i = 0; i < meshes.size(); i++)
            {
                delete meshes[i];
            }
        }
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y)
        {
            cout << button << " " << state << " " << x << " " << y << endl;
            stopTime = !stopTime;
        }

        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {
            cout << c << " " << x << " " << y << endl;
            stopTime = !stopTime;
            Vec3 zAxis = (-cameraPos).normalize();
            Vec3 up(0, 1, 0);
            Vec3 xAxis = (zAxis * up).normalize();
            Vec3 yAxis = xAxis * zAxis;

            switch (c)
            {

            case 'j':
                time -= 5;
                break;
            case 'l':
                time += 5;
                break;
            case 'a':
                cameraPos -= xAxis;
                break;
            case 's':
                cameraPos -= yAxis;
                break;
            case 'd':
                cameraPos += xAxis;
                break;
            case 'w':
                cameraPos += yAxis;
                break;
            case 'q':
                cameraPos -= zAxis;
                break;
            case 'e':
                cameraPos += zAxis;
                break;
            case 'r':
                meshPos = Vec3(-10.0, 3, 0);
                prevDir = Vec3(0.01f, 0.1f, 0);
                break;
            default:
                stopTime = !stopTime;
                break;
            }
        }

        virtual void onMouseMove(int x, int y)
        {
            float nx = x / 1024.0f;
            float ny = y / 1024.0f;
            Vec2 delta = Vec2(nx, ny) - prevMousePos;

            Vec3 zAxis = (-cameraPos).normalize();
            Vec3 up(0, 1, 0);
            Vec3 xAxis = (zAxis * up).normalize();
            Vec3 yAxis = xAxis * zAxis;

            cameraPos -= xAxis * delta.x * 50;
            cameraPos += yAxis * delta.y * 50;

            prevMousePos = Vec2(nx, ny);
        }
    private:
        Renderer* renderer;
        BlockMesh* c1;
        vector<BlockMesh*> meshes;
        unsigned int time;
        bool stopTime;
        Vec3 cameraPos;
        Vec3 meshPos;
        Vec3 prevDir;
        Scene scene;
        PerspectiveCamera camera;
        OpenGLMaterial* cubeMat;
        OpenGLMaterial* cubeTexturedMat;
        OpenGLMaterial* cubeTexturedMat2;
        Vec2 prevMousePos;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1024;
        params.height = 1024;
        params.posX = 100;
        params.posY = 100;
        params.name = "TestWindow";
        TestWindowApp app;
        Window window(params, app);
        window.startRenderLoop();
    }
}