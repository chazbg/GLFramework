#pragma once

#include "Geometry/CustomGeometry.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Demos/Demo3DBase.hpp"
#include "Core/DefaultCamera.hpp"
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
                lights[0]->Rotate(0, 3.14f / 360.0f, 0);
                lights[1]->Rotate(0, 3.14f / 360.0f, 0);
                lights[2]->Rotate(0, 3.14f / 360.0f, 0);
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

            //renderer->render(scene, camera);
            renderer->renderToTarget(scene, camera, *renderTarget);
            renderer->render(motionBlurScene, camera);
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
            lights[0]->Scale(0.05f, 0.05f, 0.05f);
            lights[0]->Translate(-sqrt(3.0f) * (r / 2.0f), 6.0f, (r / 2.0f));

            lights.push_back(geometryFactory.createCustomGeometry("3DAssets/Sphere.3ds"));
            lights[1]->setMaterial(materials[0]);
            lights[1]->Scale(0.05f, 0.05f, 0.05f);
            lights[1]->Translate(sqrt(3.0f) * (r / 2.0f), 3.0f, (r / 2.0f));

            lights.push_back(geometryFactory.createCustomGeometry("3DAssets/Sphere.3ds"));
            lights[2]->setMaterial(materials[0]);
            lights[2]->Scale(0.05f, 0.05f, 0.05f);
            lights[2]->Translate(0, 0.0f, -r);

            scene.add(lights[0].get());
            scene.add(lights[1].get());
            scene.add(lights[2].get());
        }

        void initEnvMap()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            environmentCube = geometryFactory.createCustomGeometry("3DAssets/cube.3ds", true);
            environmentCube->setMaterial(materials[1]);
            environmentCube->Scale(60, 60, 60);
            scene.add(environmentCube.get());
        }

        void initGround()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            for (unsigned int i = 0; i < 9; i++)
            {
                ground[i] = geometryFactory.createCustomGeometry("3DAssets/rectangle.3ds");
                ground[i]->setMaterial(materials[2 + i]);
                ground[i]->Rotate(-3.14f / 2.0f, 0.0f, 0.0f);
                ground[i]->Scale(10.0f, 1.0f, 10.0f);
                scene.add(ground[i].get());
            }

            ground[0]->Translate(-20.0f, -5.0f, -20.0f);
            ground[1]->Translate(0.0f, -5.0f, -20.0f);
            ground[2]->Translate(20.0f, -5.0f, -20.0f);
            ground[3]->Translate(-20.0f, -5.0f, 0.0f);
            ground[4]->Translate(0.0f, -5.0f, 0.0f);
            ground[5]->Translate(20.0f, -5.0f, 0.0f);
            ground[6]->Translate(-20.0f, -5.0f, 20.0f);
            ground[7]->Translate(0.0f, -5.0f, 20.0f);
            ground[8]->Translate(20.0f, -5.0f, 20.0f);
        }

        virtual void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/female_elf-3ds.3DS"));
            meshes[0]->Scale(0.1f, 0.1f, 0.1f);
            meshes[0]->Rotate(-3.14f / 2.0f, 0, 0);
            meshes[0]->Translate(0, -20, 0);

            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/ogrehead.obj"));
            meshes[1]->Scale(7.0f, 7.0f, 7.0f);

            /*g = geometryFactory.createCustomGeometry("3DAssets/buddha.3ds");
            g->Scale(0.01f, 0.01f, 0.01f);
            g->Rotate(-3.14f / 2.0f, 0, 0);
            g->Translate(0, 1, 0);*/

            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/hair5.obj"));
            meshes[2]->Scale(87, 85, 77);
            meshes[2]->Translate(0, -55, 0);

            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/Sphere.3ds"));
            meshes[3]->Scale(0.5, 0.5, 0.5);

            currentMesh = meshes[0];
            currentMesh->setMaterial(materials[11]);
            scene.add(currentMesh.get());

            motionBlurRect = geometryFactory.createRectangle();
            motionBlurRect->setMaterial(motionBlurMat);
            motionBlurScene.add(motionBlurRect.get());
        }

        void showNextMesh()
        {
            meshIndex = (meshIndex + 1) % 4;
            scene.remove(currentMesh.get());
            currentMesh = meshes[meshIndex];
            currentMesh->setMaterial(materials[11 + materialIndex]);
            scene.add(currentMesh.get());
        }

        void showPreviousMesh()
        {
            meshIndex = (0 == meshIndex ? 3 : (meshIndex - 1) % 4);
            scene.remove(currentMesh.get());
            currentMesh = meshes[meshIndex];
            currentMesh->setMaterial(materials[11 + materialIndex]);
            scene.add(currentMesh.get());
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
            initMaterialProperty(*materials[materialIndex], "sampler", 3);
            initMaterialProperty(*materials[materialIndex], "envMap", 4);
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

        shared_ptr<CustomGeometry> currentMesh;
        shared_ptr<CustomGeometry> environmentCube;
        shared_ptr<CustomGeometry> ground[9];
        float ior;
        float glossiness;
        vector<shared_ptr<CustomGeometry>> lights;
        vector<shared_ptr<CustomGeometry>> meshes;
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
        shared_ptr<Rectangle> motionBlurRect;
        Scene motionBlurScene;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1920;
        params.height = 1080;
        params.posX = 0;
        params.posY = 0;
        params.name = "PhysicallyBasedShading";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
                               static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}