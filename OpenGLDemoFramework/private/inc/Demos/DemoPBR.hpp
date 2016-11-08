#pragma once

#include "Math/GeometryAlgorithm.hpp"
#include "Demos/Demo3DBase.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

namespace PBRDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) : Demo3DBase(resolution) {}

        virtual void onInit()
        {
            Demo3DBase::onInit();

            initEnvMap();
            initLights();
            initGround();     

            ior = 0.01f;
            glossiness = 0.01f;
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
                lights[0]->rotate(Vec3(0.0f, 3.14f / 360.0f, 0.0f));
                lights[1]->rotate(Vec3(0.0f, 3.14f / 360.0f, 0.0f));
                lights[2]->rotate(Vec3(0.0f, 3.14f / 360.0f, 0.0f));
            }

            for (unsigned int i = 2; i < materials.size(); i++)
            {
                unsigned int pIndex = i - 2;
                materials[i]->setProperty(cameraPositions[pIndex], cameraPos);
                materials[i]->setProperty(iorProperties[pIndex], ior);
                materials[i]->setProperty(glossinessProperties[pIndex], glossiness);
                materials[i]->setProperty(lightPositions[0][pIndex], lights[0]->getPosition());
                materials[i]->setProperty(lightPositions[1][pIndex], lights[1]->getPosition());
                materials[i]->setProperty(lightPositions[2][pIndex], lights[2]->getPosition());
            }

            renderer->render(scene, camera);
            //renderer->renderToTarget(scene, camera, *renderTarget);
            //renderer->render(motionBlurScene, camera);
        }

        virtual void onKeyboardEvent(unsigned char c, int x, int y)
        {
            cout << c << " " << x << " " << y << endl;

            switch (c)
            {
            case 'q':
                showPreviousMesh();
                break;
            case 'e':
                showNextMesh();
                break;
            case 'a':
                showPreviousMaterial();
                break;
            case 'd':
                showNextMaterial();
                break;
            case 'j':
                time -= 5;
                break;
            case 'l':
                time += 5;
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
            default:
                stopTime = !stopTime;
                break;
            }
        }

        virtual void onMouseMove(int x, int y)
        {
            if (cameraPanning || cameraRotating)
            {
                Vec2 delta = Vec2(static_cast<float>(x), static_cast<float>(y)) - prevMousePos;
                delta.x /= resolution.x;
                delta.y /= resolution.y;

                motionBlurMat->setProperty(motionBlurDirProperty, delta);
            }
            
            Demo3DBase::onMouseMove(x, y);
        }
    private:

        virtual void initMaterials()
        {
            IResourceManager& resourceManager = renderer->getResourceManager();

            //0
            materials.push_back(resourceManager.createMaterial("Shaders/basicDiffuse.vs", "Shaders/basicDiffuse.fs"));
            initMaterialProperty(*materials[0], "diffuse", Vec3(1.0f, 1.0f, 0.0f));

            //1
            materials.push_back(resourceManager.createMaterial("Shaders/envMap.vs", "Shaders/envMap.fs"));
            initMaterialProperty(*materials[1], "envMap", 0);
            materials[1]->addTextureCubemap(envMap);

            //2
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/phong.fs"));
            //3
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/blinn_phong_ground.fs"));
            //4
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/blinn_phong_schlick.fs"));
            //5
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Anisotropic/ashikhmin.fs"));
            //6
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Anisotropic/kajiya_kay.fs"));
            //7
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/oren_nayar.fs"));
            //8
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/GGX_ue_normal_map.fs"));
            //9
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/GGX_schlick.fs"));
            //10
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/lambert.fs"));
           
            for (unsigned int i = 2; i < 11; i++)
            {
                initMaterialProperties(i);
                for (unsigned int j = 0; j < 3; j++)
                {
                    materials[i]->addTexture(textures[j]);
                }
                materials[i]->addTextureCubemap(envMap);
            }

            //11
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/ue.fs"));
            initMaterialProperties(11);
            materials[11]->addTexture(textures[3]);
            materials[11]->addTexture(textures[4]);
            materials[11]->addTexture(textures[4]);
            materials[11]->addTextureCubemap(envMap);

            //12
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/GGX_ue.fs"));
            initMaterialProperties(12);
            materials[12]->addTexture(textures[5]);
            materials[12]->addTexture(textures[6]);
            materials[12]->addTexture(textures[6]);
            materials[12]->addTextureCubemap(envMap);

            //13
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Anisotropic/kajiya_kay_ogre.fs"));
            initMaterialProperties(13);
            materials[13]->addTexture(textures[5]);
            materials[13]->addTexture(textures[6]);
            materials[13]->addTexture(textures[6]);
            materials[13]->addTextureCubemap(envMap);

            //14
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Anisotropic/ashikhmin_body.fs"));
            initMaterialProperties(14);
            materials[14]->addTextureCubemap(envMap);

            //---
            motionBlurMat = resourceManager.createMaterial("Shaders/tex.vs", "Shaders/motionBlur.fs");
            motionBlurMat->getProperty("motionBlurDir", motionBlurDirProperty);
            motionBlurMat->addTexture(offscreenTexture);
            initMaterialProperty(*motionBlurMat, "sampler", 0);
        }

        virtual void initTextures()
        {
            IResourceManager& rm = renderer->getResourceManager();
            envMap = rm.createTextureCubemap(
                "Images/cubemap_1/posz.png",
                "Images/cubemap_1/negz.png",
                "Images/cubemap_1/posy.png",
                "Images/cubemap_1/negy.png",
                "Images/cubemap_1/posx.png",
                "Images/cubemap_1/negx.png"
                );

            textures.push_back(rm.createTexture("Images/pattern_09/diffuse.png"));
            textures.push_back(rm.createTexture("Images/pattern_09/normal.png"));
            textures.push_back(rm.createTexture("Images/pattern_09/specular.png"));
            textures.push_back(rm.createTexture("Images/pattern6/diffuse2.jpg"));
            textures.push_back(rm.createTexture("Images/pattern6/specular.jpg"));
            textures.push_back(rm.createTexture("Images/ogrehead_diffuse.png"));
            textures.push_back(rm.createTexture("Images/ogre_normalmap.png"));

            offscreenTexture = rm.createTexture(
                static_cast<unsigned int>(resolution.x),
                static_cast<unsigned int>(resolution.y), 3, false);
            renderTarget = rm.createRenderTarget();
            renderTarget->addColorTexture(offscreenTexture);
            renderTarget->addDepthTexture(rm.createTexture(
                static_cast<unsigned int>(resolution.x),
                static_cast<unsigned int>(resolution.y), 3, true));
        }

        void initLights()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            float r = 20.0f;
            lights.push_back(geometryFactory.createCustomGeometry("3DAssets/Sphere.3ds"));
            lights[0]->setMaterial(materials[0]);
            lights[0]->scale(Vec3(0.05f));
            lights[0]->translate(Vec3(-sqrt(3.0f) * (r / 2.0f), 6.0f, (r / 2.0f)));

            lights.push_back(geometryFactory.createCustomGeometry("3DAssets/Sphere.3ds"));
            lights[1]->setMaterial(materials[0]);
            lights[1]->scale(Vec3(0.05f));
            lights[1]->translate(Vec3(sqrt(3.0f) * (r / 2.0f), 3.0f, (r / 2.0f)));

            lights.push_back(geometryFactory.createCustomGeometry("3DAssets/Sphere.3ds"));
            lights[2]->setMaterial(materials[0]);
            lights[2]->scale(Vec3(0.05f));
            lights[2]->translate(Vec3(0.0f, 0.0f, -r));

            scene.add(lights[0]);
            scene.add(lights[1]);
            scene.add(lights[2]);
        }

        void initEnvMap()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            environmentCube = geometryFactory.createCustomGeometry("3DAssets/cube.3ds", true);
            environmentCube->setMaterial(materials[1]);
            environmentCube->scale(Vec3(60.0f));
            scene.add(environmentCube);
        }

        void initGround()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            for (unsigned int i = 0; i < 9; i++)
            {
                ground[i] = geometryFactory.createCustomGeometry("3DAssets/rectangle.3ds");
                ground[i]->setMaterial(materials[2 + i]);
                ground[i]->rotate(Vec3(-3.14f / 2.0f, 0.0f, 0.0f));
                ground[i]->scale(Vec3(10.0f, 1.0f, 10.0f));
                scene.add(ground[i]);
            }

            ground[0]->translate(Vec3(-20.0f, -5.0f, -20.0f));
            ground[1]->translate(Vec3(0.0f, -5.0f, -20.0f));
            ground[2]->translate(Vec3(20.0f, -5.0f, -20.0f));
            ground[3]->translate(Vec3(-20.0f, -5.0f, 0.0f));
            ground[4]->translate(Vec3(0.0f, -5.0f, 0.0f));
            ground[5]->translate(Vec3(20.0f, -5.0f, 0.0f));
            ground[6]->translate(Vec3(-20.0f, -5.0f, 20.0f));
            ground[7]->translate(Vec3(0.0f, -5.0f, 20.0f));
            ground[8]->translate(Vec3(20.0f, -5.0f, 20.0f));
        }

        virtual void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/female_elf-3ds.3DS"));
            meshes[0]->scale(Vec3(0.1f));
            meshes[0]->rotate(Vec3(-3.14f / 2.0f, 0.0f, 0.0f));
            meshes[0]->translate(Vec3(-1.0f, 5.0f, 2.0f));

            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/ogrehead.obj"));
            meshes[1]->scale(Vec3(7.0f));

            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/hair5.obj"));
            meshes[2]->scale(Vec3(87, 85, 77));
            meshes[2]->translate(Vec3(0, -55, 0));

            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/Sphere.3ds"));
            meshes[3]->scale(Vec3(0.5f));

            currentMesh = meshes[0];
            currentMesh->setMaterial(materials[11]);
            scene.add(currentMesh);

            motionBlurRect = geometryFactory.createRectangle();
            motionBlurRect->setMaterial(motionBlurMat);
            motionBlurScene.add(motionBlurRect);
        }

        void showNextMesh()
        {
            meshIndex = (meshIndex + 1) % 4;
            scene.remove(currentMesh);
            currentMesh = meshes[meshIndex];
            currentMesh->setMaterial(materials[11 + materialIndex]);
            scene.add(currentMesh);
        }

        void showPreviousMesh()
        {
            meshIndex = (0 == meshIndex ? 3 : (meshIndex - 1) % 4);
            scene.remove(currentMesh);
            currentMesh = meshes[meshIndex];
            currentMesh->setMaterial(materials[11 + materialIndex]);
            scene.add(currentMesh);
        }

        void showNextMaterial()
        {
            materialIndex = (materialIndex + 1) % 4;
            currentMesh->setMaterial(materials[11 + materialIndex]);
        }

        void showPreviousMaterial()
        {
            materialIndex = (0 == materialIndex ? 3 : (materialIndex - 1) % 4);
            currentMesh->setMaterial(materials[11 + materialIndex]);
        }

        void initMaterialProperties(const int materialIndex)
        {
            initMaterialProperty(*materials[materialIndex], "diffuseMap", 0);
            initMaterialProperty(*materials[materialIndex], "normalMap", 1);
            initMaterialProperty(*materials[materialIndex], "specMap", 2);
            initMaterialProperty(*materials[materialIndex], "envMap", 3);
            initMaterialProperty(*materials[materialIndex], "diffuse", Vec3(0.5f, 0.5f, 0.5f));
            initMaterialProperty(*materials[materialIndex], "specular", Vec3(1.0f, 0.71f, 0.29f));

            Vec3PropertySharedPtr v3p;
            FloatPropertySharedPtr fp;
            materials[materialIndex]->getProperty("cameraPos", v3p);
            cameraPositions.push_back(v3p);
            materials[materialIndex]->getProperty("light0Pos", v3p);
            lightPositions[0].push_back(v3p);
            materials[materialIndex]->getProperty("light1Pos", v3p);
            lightPositions[1].push_back(v3p);
            materials[materialIndex]->getProperty("light2Pos", v3p);
            lightPositions[2].push_back(v3p);

            materials[materialIndex]->getProperty("ior", fp);
            iorProperties.push_back(fp);

            materials[materialIndex]->getProperty("glossiness", fp);
            glossinessProperties.push_back(fp);            
        }

        std::shared_ptr<MeshNode> currentMesh;
        std::shared_ptr<MeshNode> environmentCube;
        std::shared_ptr<MeshNode> ground[9];
        float ior;
        float glossiness;
        vector<std::shared_ptr<MeshNode>> lights;
        vector<std::shared_ptr<MeshNode>> meshes;
        vector<IMaterial*> materials;
        vector<Vec3PropertySharedPtr> cameraPositions;
        vector<Vec3PropertySharedPtr> lightPositions[3];
        vector<FloatPropertySharedPtr> iorProperties;
        vector<FloatPropertySharedPtr> glossinessProperties;
        vector<ITexture*> textures;
        ITextureCubemap* envMap;
        int materialIndex;
        int meshIndex;
        IRenderTarget* renderTarget;
        ITexture* offscreenTexture;
        IMaterial* motionBlurMat;
        Vec2PropertySharedPtr motionBlurDirProperty;
        shared_ptr<MeshNode> motionBlurRect;
        Scene motionBlurScene;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1600;
        params.height = 900;
        params.posX = 0;
        params.posY = 0;
        params.name = "PhysicallyBasedShading";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}