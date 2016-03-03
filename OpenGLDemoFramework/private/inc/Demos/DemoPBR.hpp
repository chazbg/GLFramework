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

		void initMaterials()
		{
            //0
			materials.push_back(new ShaderMaterial("Shaders/basicDiffuse.vs", "Shaders/basicDiffuse.fs"));
			materials[0]->setProperty("diffuse", Vec3(1.0f, 1.0f, 0.0f));

            //1
			materials.push_back(new ShaderMaterial("Shaders/envMap.vs", "Shaders/envMap.fs"));
			materials[1]->setProperty("envMap", 1);
			materials[1]->addTextureCubemap(envMap);

            //2
			materials.push_back(new ShaderMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/GGX_ue.fs"));
            //3
			materials.push_back(new ShaderMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/phong.fs"));
            //4
			materials.push_back(new ShaderMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/blinn_phong_schlick.fs"));
            //5
			materials.push_back(new ShaderMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/oren_nayar.fs"));
            //6
			materials.push_back(new ShaderMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Anisotropic/ashikhmin.fs"));
            //7
			materials.push_back(new ShaderMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Isotropic/blinn_phong_ground.fs"));
            //8
			materials.push_back(new ShaderMaterial("Shaders/BRDF/Isotropic/semiGGX.vs", "Shaders/BRDF/Anisotropic/kajiya_kay.fs"));			

			for (unsigned int i = 2; i < materials.size(); i++)
			{
				materials[i]->setProperty("diffuseMap", 0);
				materials[i]->setProperty("normalMap", 1);
				materials[i]->setProperty("specMap", 2);
				materials[i]->setProperty("sampler", 3);
				materials[i]->setProperty("envMap", 4);
				materials[i]->setProperty("diffuse", Vec3(0.5f, 0.5f, 0.5f));
				materials[i]->setProperty("specular", Vec3(1.0f, 0.71f, 0.29f));
				for (unsigned int j = 0; j < textures.size(); j++)
				{
					materials[i]->addTexture(textures[j]);
				}
				materials[i]->addTextureCubemap(envMap);
			}
		}

		void initTextures()
		{
			TextureLoader texLoader;

			envMap = texLoader.loadTextureCubemap(
				"Images/cubemap_0/Cubemap_Back.png",
				"Images/cubemap_0/Cubemap_Front.png",
				"Images/cubemap_0/Cubemap_Top.png",
				"Images/cubemap_0/Cubemap_Bottom.png",
				"Images/cubemap_0/Cubemap_Right.png",
				"Images/cubemap_0/Cubemap_Left.png"
				);

			textures.push_back(texLoader.loadTexture("Images/pattern5/diffuse.jpg"));
			textures.push_back(texLoader.loadTexture("Images/pattern_124/normal.png"));
			textures.push_back(texLoader.loadTexture("Images/pattern5/specular.jpg"));
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
            environmentCube->Scale(30, 30, 30);
            scene.add(environmentCube);
        }

        void initGround()
        {
			for (unsigned int i = 0; i < 9; i++)
			{
				ground[i] = new CustomGeometry("3DAssets/rectangle.3ds");
				ground[i]->setMaterial(materials[3 + i % 6]);
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
			g1 = new CustomGeometry("3DAssets/female_elf-3ds.3DS");
			g1->Scale(0.1f, 0.1f, 0.1f);
			g1->Rotate(-3.14f / 2.0f, 0, 0);
			g1->Translate(0, -20, 0);

			/*g = new CustomGeometry("3DAssets/ogrehead.obj");
			g->Scale(7.0f, 7.0f, 7.0f);*/

			/*g = new CustomGeometry("3DAssets/buddha.3ds");
			g->Scale(0.01f, 0.01f, 0.01f);
			g->Rotate(-3.14f / 2.0f, 0, 0);
			g->Translate(0, 1, 0);*/

            //g = new CustomGeometry("3DAssets/hair3_triangulated.obj");
            //g->Scale(17.0f, 16.0f, 15.0f);
            ////g->Rotate(-3.14f / 2.0f, 0, 0);
            //g->Translate(0.5, -14, 0.0);

            g1->setMaterial(materials[3]);
            scene.add(g1);

			/*g->setMaterial(materials[8]);
			scene.add(g);*/
		}

        PerspectiveCamera camera;
        Scene scene;
        Renderer* renderer;
        CustomGeometry* g;
        CustomGeometry* g1;
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
		vector<IMaterial*> materials;
		vector<Texture*> textures;
		TextureCubemap* envMap;
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