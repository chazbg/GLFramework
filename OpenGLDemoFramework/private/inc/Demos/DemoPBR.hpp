#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include <Core/PerspectiveCamera.hpp>
#include <Core/Scene.hpp>
#include <Core/ShaderMaterial.hpp>
#include <Core/TextureLoader.hpp>
#include <Geometry/CustomGeometry.hpp>
#include <Math/GeometryAlgorithm.hpp>
#include <iostream>
#include <algorithm>

using namespace std;

namespace PBRDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp() : renderer(0) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            TextureLoader texLoader;

            renderer = new Renderer(Vec2(800, 800));

            envMap = texLoader.loadTextureCubemap(
                "Images/cubemap_miramar/miramar_posz.png",
                "Images/cubemap_miramar/miramar_negz.png",
                "Images/cubemap_miramar/miramar_posy.png",
                "Images/cubemap_miramar/miramar_negy.png",
                "Images/cubemap_miramar/miramar_posx.png",
                "Images/cubemap_miramar/miramar_negx.png");

            shaderMaterial = new ShaderMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/semiGGX.fs");
            shaderMaterial->addTexture(texLoader.loadTexture("Images/Football.png"));
            shaderMaterial->addTextureCubemap(envMap);

            shaderMaterial->setProperty("colorMap", 0);
            shaderMaterial->setProperty("sampler", 1);
            shaderMaterial->setProperty("envMap", 2);
            shaderMaterial->setProperty("diffuse", Vec3(0.5f, 0.5f, 0.5f));
            shaderMaterial->setProperty("specular", Vec3(1.0f, 0.71f, 0.29f));

            //g = new CustomGeometry("3DAssets/female_elf-3ds.3DS");
            //g->Scale(0.1f, 0.1f, 0.1f);
            //g->Rotate(-3.14f / 2.0f, 0, 0);
            //g->Translate(0, -20, 0);

            //g = new CustomGeometry("3DAssets/ogrehead.obj");
            //g->Scale(7.0f, 7.0f, 7.0f);

            g = new CustomGeometry("3DAssets/buddha.3ds");
            g->Scale(0.01f, 0.01f, 0.01f);
            g->Rotate(-3.14f / 2.0f, 0, 0);
            g->Translate(0, 1, 0);

            g->setMaterial(shaderMaterial);

            initEnvMap();
            initLights();
            initGround();

            scene.add(g);
            
            time = 0;
            stopTime = false;
            prevDir = Vec3(0.01f, 0.1f, 0);
            prevMousePos = Vec2(0.5, 0.5);
            phi = 3.14f / 2.0f;
            theta = 0;
            radius = 30;
            float t = radius * cos(theta);
            cameraPos = Vec3(t * cos(phi), radius * sin(theta), t * sin(phi));
            ior = 0.0f;
            glossiness = 0.0f;
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            camera.setPosition(cameraPos);

            if (!stopTime)
            {
                time++;
                lights[0]->Rotate(0, 3.14f / 360.0f, 0);
                lights[1]->Rotate(0, 3.14f / 360.0f, 0);
                lights[2]->Rotate(0, 3.14f / 360.0f, 0);
            }

            g->getMaterial().setProperty("cameraPos", cameraPos);
            g->getMaterial().setProperty("ior", ior);
            g->getMaterial().setProperty("glossiness", glossiness);
            g->getMaterial().setProperty("light0Pos", lights[0]->getPosition());
            g->getMaterial().setProperty("light1Pos", lights[1]->getPosition());
            g->getMaterial().setProperty("light2Pos", lights[2]->getPosition());

            ground->getMaterial().setProperty("cameraPos", cameraPos);
            ground->getMaterial().setProperty("ior", ior);
            ground->getMaterial().setProperty("glossiness", glossiness);
            ground->getMaterial().setProperty("light0Pos", lights[0]->getPosition());
            ground->getMaterial().setProperty("light1Pos", lights[1]->getPosition());
            ground->getMaterial().setProperty("light2Pos", lights[2]->getPosition());

            renderer->render(scene, camera);
        }

        virtual void onDestroy() { delete g; }
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {
            cout << c << " " << x << " " << y << endl;

            switch (c)
            {

            case 'j':
                time -= 5;
                break;
            case 'l':
                time += 5;
                break;
            case 'a':
                phi -= 0.1f;
                break;
            case 's':
                theta -= 0.1f;
                break;
            case 'd':
                phi += 0.1f;
                break;
            case 'w':
                theta += 0.1f;
                break;
            case 'q':
                radius += 0.5f;
                break;
            case 'e':
                radius -= 0.5f;
                break;
            case '[':
                ior = max(0.000001f, ior - 0.01f);
                cout << "ior: " << ior << endl;
                break;
            case ']':
                ior = min(0.999999f, ior + 0.01f);
                cout << "ior: " << ior << endl;
                break;
            case '-':
                glossiness = max(0.01f, glossiness - 0.01f);
                cout << "glossiness: " << glossiness << endl;
                break;
            case '=':
                glossiness = min(0.99f, glossiness + 0.01f);
                cout << "glossiness: " << glossiness << endl;
                break;
            case '1':
                ground->setMaterial(anisotropicMaterial);
                break;
            case '2':
                ground->setMaterial(shaderMaterial);
                break;
            default:
                stopTime = !stopTime;
                break;
            }
            float t = radius * cos(theta);
            cameraPos = Vec3(t * cos(phi), radius * sin(theta), t * sin(phi));
        }

        virtual void onMouseMove(int x, int y)
        {
            /*float nx = x / 800.0f;
            float ny = y / 800.0f;
            Vec2 delta = Vec2(nx, ny) - prevMousePos;

            Vec3 zAxis = (-cameraPos).normalize();
            Vec3 up(0, 1, 0);
            Vec3 xAxis = (zAxis * up).normalize();
            Vec3 yAxis = xAxis * zAxis;

            cameraPos -= xAxis * delta.x * 50;
            cameraPos += yAxis * delta.y * 50;

            prevMousePos = Vec2(nx, ny);*/
        }
    private:

        void initLights()
        {
            lightMeshMaterial = new ShaderMaterial("Shaders/basicDiffuse.vs", "Shaders/basicDiffuse.fs");
            lightMeshMaterial->setProperty("diffuse", Vec3(1.0f, 1.0f, 0.0f));

            lights.push_back(new CustomGeometry("3DAssets/Sphere.3ds"));
            lights[0]->setMaterial(lightMeshMaterial);
            lights[0]->Scale(0.05f, 0.05f, 0.05f);
            lights[0]->Translate(-sqrt(3.0f)*2.5f, 3.0f, 2.5f);

            lights.push_back(new CustomGeometry("3DAssets/Sphere.3ds"));
            lights[1]->setMaterial(lightMeshMaterial);
            lights[1]->Scale(0.05f, 0.05f, 0.05f);
            lights[1]->Translate(sqrt(3.0f)*2.5f, 3.0f, 2.5f);

            lights.push_back(new CustomGeometry("3DAssets/Sphere.3ds"));
            lights[2]->setMaterial(lightMeshMaterial);
            lights[2]->Scale(0.05f, 0.05f, 0.05f);
            lights[2]->Translate(0, 3.0f, -5.0f);

            scene.add(lights[0]);
            scene.add(lights[1]);
            scene.add(lights[2]);
        }

        void initEnvMap()
        {
            envMapMaterial = new ShaderMaterial("Shaders/envMap.vs", "Shaders/envMap.fs");
            envMapMaterial->setProperty("envMap", 1);

            environmentCube = new CustomGeometry("3DAssets/cube.3ds", true);
            environmentCube->setMaterial(envMapMaterial);
            environmentCube->Scale(20, 20, 20);
            scene.add(environmentCube);

            envMapMaterial->addTextureCubemap(envMap);
        }

        void initGround()
        {
            anisotropicMaterial = new ShaderMaterial("Shaders/BRDF/Anisotropic/anisotropicBRDF.vs", "Shaders/BRDF/Anisotropic/anisotropicBRDF.fs");
            shaderMaterial->setProperty("colorMap", 0);
            shaderMaterial->setProperty("sampler", 1);
            shaderMaterial->setProperty("envMap", 2);
            shaderMaterial->setProperty("diffuse", Vec3(0.5f, 0.5f, 0.5f));
            shaderMaterial->setProperty("specular", Vec3(1.0f, 0.71f, 0.29f));

            ground = new CustomGeometry("3DAssets/cube.3ds");
            ground->setMaterial(anisotropicMaterial);
            ground->Translate(0.0f, -5.0f, 0.0f);
            ground->Scale(20.0f, 1.0f, 20.0f);
            scene.add(ground);

            anisotropicMaterial->addTextureCubemap(envMap);
        }

        PerspectiveCamera camera;
        Scene scene;
        Renderer* renderer;
        CustomGeometry* g;
        CustomGeometry* environmentCube;
        CustomGeometry* ground;
        ShaderMaterial* shaderMaterial;
        ShaderMaterial* lightMeshMaterial;
        ShaderMaterial* envMapMaterial;
        ShaderMaterial* anisotropicMaterial;
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
        vector<CustomGeometry*> lights;
        TextureCubemap* envMap;
    };

    void main()
    {
        WindowParameters params;
        params.width = 800;
        params.height = 800;
        params.posX = 100;
        params.posY = 100;
        params.name = "TestWindow";
        TestWindowApp app;
        Window window(params, app);
        window.startRenderLoop();
    }
}