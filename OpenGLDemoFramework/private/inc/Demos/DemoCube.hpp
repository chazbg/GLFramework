#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Core/Scene.hpp"
#include "Core/PerspectiveCamera.hpp"

namespace CubeDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp(const Vec2 resolution) : 
            resolution(resolution), 
            camera(3.0f / 4.0f, 16.0f / 9.0f, 1.0f, 1000.0f), 
            renderer(0),
            cubesCount(30) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer(resolution);

            initTextures();
            initMaterials();
            initGeometry();

            time = 0;
            stopTime = false;
            prevDir = Vec3(0.01f, 0.1f, 0);
            prevMousePos = Vec2(0.5, 0.5);
            phi = 3.14f / 2.0f;
            theta = 0;
            radius = 30;
            float t = radius * cos(theta);
            cameraPos = Vec3(t * cos(phi), radius * sin(theta), t * sin(phi));

            cubeVertices[0] = Vec3(0, 0, 0);
            cubeVertices[1] = Vec3(1, 0, 0);
            cubeVertices[2] = Vec3(1, 1, 0);
            cubeVertices[3] = Vec3(1, 0, 1);
            cubeVertices[4] = Vec3(1, 1, 1);
            cubeVertices[5] = Vec3(0, 1, 0);
            cubeVertices[6] = Vec3(0, 1, 1);
            cubeVertices[7] = Vec3(0, 0, 1);

            barycentricCoords[0] = 1.0f;
            barycentricCoords[1] = 0.0f;
            barycentricCoords[2] = 0.0f;
            barycentricCoords[3] = 0.0f;
            barycentricCoords[4] = 0.0f;
            barycentricCoords[5] = 0.0f;
            barycentricCoords[6] = 0.0f;
            barycentricCoords[7] = 0.0f;

            interpolationSteps = 30;
            interpolator = 0;
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            camera.setPosition(cameraPos);

            if (!stopTime)
            {
                time++;

                float delta = 1.0f / interpolationSteps;
                int index = (interpolator / interpolationSteps) % 8;
                Vec3 newPos = cubeVertices[index] * barycentricCoords[index] * 5.0f +
                    cubeVertices[(index + 1) % 8] * barycentricCoords[(index + 1) % 8] * 5.0f;

                for (unsigned int i = cubesCount - 1; i >= 1; i--)
                {
                    Vec3 translation = cubes[i - 1].getPosition() - cubes[i].getPosition();
                    BlockMesh* cube = &(*(cubes.begin() + i));
                    cube->Translate(translation.x, translation.y, translation.z);
                }

                BlockMesh* cube = &(*cubes.begin());
                Vec3 translation = newPos - cube->getPosition();
                cube->Translate(translation.x, translation.y, translation.z);

                interpolator++;
                barycentricCoords[index] -= delta;
                barycentricCoords[(index + 1) % 8] += delta;
            }

            renderer->render(scene, camera);
        }

        virtual void onDestroy() {}
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y)
        {
            cout << "onMouseEvent: " << button << " " << state << endl;
            if (0 == button)
            {
                cameraRotating = (0 == state ? true : false);
            }

            if (2 == button)
            {
                cameraPanning = (0 == state ? true : false);
            }

            if (3 == button)
            {
                radius -= 1.0f;
                updateCamera();
            }

            if (4 == button)
            {
                radius += 1.0f;
                updateCamera();
            }
        }

        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {
            cout << static_cast<int>(c) << " " << x << " " << y << endl;

            switch (c)
            {
            default:
                stopTime = !stopTime;
                break;
            }
        }

        virtual void onMouseMove(int x, int y)
        {
            Vec2 delta = Vec2(static_cast<float>(x), static_cast<float>(y)) - prevMousePos;
            delta.x /= resolution.x;
            delta.y /= resolution.y;
            if (cameraRotating)
            {
                phi += 3.14f * delta.x;
                theta += 3.14f * delta.y;
                updateCamera();
            }

            if (cameraPanning)
            {
                Vec3 center = camera.getLookDirection();
                Vec3 z = center - cameraPos;
                Vec3 y = camera.getUpVector();
                Vec3 dx = (z * y).normalize();
                Vec3 dy = (z * dx).normalize();

                cameraPos += (dx * delta.x + dy * delta.y) * 30.0f;
                camera.setLookDirection(center + (dx * delta.x + dy * delta.y) * 30.0f);
            }

            prevMousePos = Vec2(static_cast<float>(x), static_cast<float>(y));
        }
    private:
        void initTextures()
        {
            IResourceManager& rm = renderer->getResourceManager();

            textures.push_back(rm.createTexture("Images/pattern_133/diffuse.png"));
        }

        void initMaterials()
        {
            IResourceManager& resourceManager = renderer->getResourceManager();

            //0
            materials.push_back(resourceManager.createMaterial(
                "Shaders/basicDiffuse.vs",
                "Shaders/basicDiffuse.fs"));
            materials[0]->setProperty("sampler", 0);
            materials[0]->setProperty("diffuse", Vec3(1, 1, 0));

            //1
            materials.push_back(resourceManager.createMaterial(
                "Shaders/basicDiffuse.vs",
                "Shaders/basicDiffuse.fs",
                "Shaders/wireframe.gs"));
            materials[1]->setProperty("diffuse", Vec3(0.5f, 0.0f, 0.0f));
        }

        void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();

            cubes.reserve(cubesCount + 1);

            for (unsigned int i = 0; i < cubesCount; i++)
            {
                cubes.push_back(geometryFactory.createBlockMesh());
                BlockMesh* cube = &(*(cubes.begin() + i));
                const float baseSize = 0.2f;
                const float scale = 0.2f + (static_cast<float>(cubesCount - i) / cubesCount) * 0.1f;
                cube->Scale(scale, scale, scale);
                cube->setMaterial(materials[0]);
                scene.add(cube);
            }     

            cubes.push_back(geometryFactory.createBlockMesh());
            BlockMesh* cube = &(*(cubes.begin() + cubesCount));
            cube->Translate(0.5f, 0.5f, 0.5f);
            cube->Scale(5.0f, 5.0f, 5.0f);
            cube->setMaterial(materials[1]);
            scene.add(cube);
        }

        void updateCamera()
        {
            float t = radius * cos(theta);
            cameraPos = camera.getLookDirection() + Vec3(t * cos(phi), radius * sin(theta), t * sin(phi));
        }

        const Vec2 resolution;
        PerspectiveCamera camera;
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
        vector<BlockMesh> cubes;
        vector<IMaterial*> materials;
        vector<ITexture*> textures;
        ITextureCubemap* envMap;
        bool cameraRotating;
        bool cameraPanning;
        const unsigned int cubesCount;
        Vec3 cubeVertices[8];
        float barycentricCoords[8];
        int interpolationSteps;
        int interpolator;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1280;
        params.height = 720;
        params.posX = 0;
        params.posY = 0;
        params.name = "Cube";
        TestWindowApp app(Vec2(static_cast<float>(params.width), 
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}