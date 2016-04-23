#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include <Core/PerspectiveCamera.hpp>
#include <Core/Scene.hpp>
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
        TestWindowApp() : camera(3.0f / 4.0f, 16.0f / 9.0f, 1.0f, 1000.0f), renderer(0) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer(Vec2(1920, 1080));

			initTextures();
			initMaterials();
            initEnvMap();
            initLights();
            initGround();
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
				materials[i]->setProperty("cameraPos", cameraPos);
				materials[i]->setProperty("ior", ior);
				materials[i]->setProperty("glossiness", glossiness);
				materials[i]->setProperty("light0Pos", lights[0]->getPosition());
				materials[i]->setProperty("light1Pos", lights[1]->getPosition());
				materials[i]->setProperty("light2Pos", lights[2]->getPosition());
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
            Vec2 delta = Vec2(static_cast<float>(x), static_cast<float>(y)) - prevMousePos;
            delta.x /= 1920.0f;
            delta.y /= 1080.0f;
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

		void initMaterials()
		{
            IResourceManager& resourceManager = renderer->getResourceManager();

            //0
			materials.push_back(resourceManager.createMaterial("Shaders/basicDiffuse.vs", "Shaders/basicDiffuse.fs"));
			materials[0]->setProperty("diffuse", Vec3(1.0f, 1.0f, 0.0f));

            //1
			materials.push_back(resourceManager.createMaterial("Shaders/envMap.vs", "Shaders/envMap.fs"));
			materials[1]->setProperty("envMap", 1);
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
				materials[i]->setProperty("diffuseMap", 0);
				materials[i]->setProperty("normalMap", 1);
				materials[i]->setProperty("specMap", 2);
				materials[i]->setProperty("sampler", 3);
				materials[i]->setProperty("envMap", 4);
				materials[i]->setProperty("diffuse", Vec3(0.5f, 0.5f, 0.5f));
				materials[i]->setProperty("specular", Vec3(1.0f, 0.71f, 0.29f));
				for (unsigned int j = 0; j < 3; j++)
				{
					materials[i]->addTexture(textures[j]);
				}
				materials[i]->addTextureCubemap(envMap);
			}

            //11
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/ue.fs"));
            materials[11]->setProperty("diffuseMap", 0);
            materials[11]->setProperty("normalMap", 1);
            materials[11]->setProperty("specMap", 2);
            materials[11]->setProperty("sampler", 3);
            materials[11]->setProperty("envMap", 4);
            materials[11]->setProperty("diffuse", Vec3(0.5f, 0.5f, 0.5f));
            materials[11]->setProperty("specular", Vec3(1.0f, 0.71f, 0.29f));
            materials[11]->addTexture(textures[3]);
            materials[11]->addTexture(textures[4]);
            materials[11]->addTexture(textures[4]);
            materials[11]->addTextureCubemap(envMap);

            //12
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/GGX_ue.fs"));
            materials[12]->setProperty("diffuseMap", 0);
            materials[12]->setProperty("normalMap", 1);
            materials[12]->setProperty("specMap", 2);
            materials[12]->setProperty("sampler", 3);
            materials[12]->setProperty("envMap", 4);
            materials[12]->setProperty("diffuse", Vec3(0.5f, 0.5f, 0.5f));
            materials[12]->setProperty("specular", Vec3(1.0f, 0.71f, 0.29f));
            materials[12]->addTexture(textures[5]);
            materials[12]->addTexture(textures[6]);
            materials[12]->addTexture(textures[6]);
            materials[12]->addTextureCubemap(envMap);

			//13
			materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Anisotropic/kajiya_kay_ogre.fs"));
			materials[13]->setProperty("diffuseMap", 0);
			materials[13]->setProperty("normalMap", 1);
			materials[13]->setProperty("specMap", 2);
			materials[13]->setProperty("sampler", 3);
			materials[13]->setProperty("envMap", 4);
			materials[13]->setProperty("diffuse", Vec3(0.5f, 0.5f, 0.5f));
			materials[13]->setProperty("specular", Vec3(1.0f, 0.71f, 0.29f));
			materials[13]->addTexture(textures[5]);
			materials[13]->addTexture(textures[6]);
			materials[13]->addTexture(textures[6]);
			materials[13]->addTextureCubemap(envMap);

            //14
            materials.push_back(resourceManager.createMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Anisotropic/ashikhmin_body.fs"));
            materials[14]->setProperty("diffuseMap", 0);
            materials[14]->setProperty("normalMap", 1);
            materials[14]->setProperty("specMap", 2);
            materials[14]->setProperty("sampler", 3);
            materials[14]->setProperty("envMap", 4);
            materials[14]->setProperty("diffuse", Vec3(0.5f, 0.5f, 0.5f));
            materials[14]->setProperty("specular", Vec3(1.0f, 0.71f, 0.29f));
            materials[14]->addTextureCubemap(envMap);
		}

		void initTextures()
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
		}

        void initLights()
        {
			float r = 20.0f;
            lights.push_back(new CustomGeometry("3DAssets/Sphere.3ds"));
			lights[0]->setMaterial(materials[0]);
            lights[0]->Scale(0.05f, 0.05f, 0.05f);
			lights[0]->Translate(-sqrt(3.0f) * (r / 2.0f), 6.0f, (r / 2.0f));

            lights.push_back(new CustomGeometry("3DAssets/Sphere.3ds"));
			lights[1]->setMaterial(materials[0]);
            lights[1]->Scale(0.05f, 0.05f, 0.05f);
			lights[1]->Translate(sqrt(3.0f) * (r / 2.0f), 3.0f, (r / 2.0f));

            lights.push_back(new CustomGeometry("3DAssets/Sphere.3ds"));
			lights[2]->setMaterial(materials[0]);
            lights[2]->Scale(0.05f, 0.05f, 0.05f);
			lights[2]->Translate(0, 0.0f, -r);

            scene.add(lights[0]);
            scene.add(lights[1]);
            scene.add(lights[2]);
        }

        void initEnvMap()
        {
            environmentCube = new CustomGeometry("3DAssets/cube.3ds", true);
			environmentCube->setMaterial(materials[1]);
            environmentCube->Scale(60, 60, 60);
            scene.add(environmentCube);
        }

        void initGround()
        {
			for (unsigned int i = 0; i < 9; i++)
			{
				ground[i] = new CustomGeometry("3DAssets/rectangle.3ds");
				ground[i]->setMaterial(materials[2 + i]);
				ground[i]->Rotate(-3.14f / 2.0f, 0.0f, 0.0f);
				ground[i]->Scale(10.0f, 1.0f, 10.0f);
				scene.add(ground[i]);
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

		void initGeometry()
		{
			meshes.push_back(new CustomGeometry("3DAssets/female_elf-3ds.3DS"));
			meshes[0]->Scale(0.1f, 0.1f, 0.1f);
			meshes[0]->Rotate(-3.14f / 2.0f, 0, 0);
			meshes[0]->Translate(0, -20, 0);

			meshes.push_back(new CustomGeometry("3DAssets/ogrehead.obj"));
			meshes[1]->Scale(7.0f, 7.0f, 7.0f);

			/*g = new CustomGeometry("3DAssets/buddha.3ds");
			g->Scale(0.01f, 0.01f, 0.01f);
			g->Rotate(-3.14f / 2.0f, 0, 0);
			g->Translate(0, 1, 0);*/

			meshes.push_back(new CustomGeometry("3DAssets/hair5.obj"));
            meshes[2]->Scale(87, 85, 77);
            meshes[2]->Translate(0, -55, 0);

            meshes.push_back(new CustomGeometry("3DAssets/Sphere.3ds"));
            meshes[3]->Scale(0.5, 0.5, 0.5);

			currentMesh = meshes[0];
			currentMesh->setMaterial(materials[11]);
			scene.add(currentMesh);
		}

        void updateCamera()
        {
            float t = radius * cos(theta);
            cameraPos = camera.getLookDirection() + Vec3(t * cos(phi), radius * sin(theta), t * sin(phi));
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

        PerspectiveCamera camera;
        Scene scene;
        Renderer* renderer;
        CustomGeometry* currentMesh;
        CustomGeometry* environmentCube;
        CustomGeometry* ground[9];
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
		vector<CustomGeometry*> meshes;
		vector<IMaterial*> materials;
		vector<ITexture*> textures;
		ITextureCubemap* envMap;
        bool cameraRotating;
        bool cameraPanning;
		int materialIndex;
		int meshIndex;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1920;
        params.height = 1080;
        params.posX = 0;
        params.posY = 0;
        params.name = "PhysicallyBasedShading";
        TestWindowApp app;
        Window window(params, app);
        window.startRenderLoop();
    }
}