#pragma once

#include "Demos/Demo3DBase.hpp"

using namespace std;

namespace PlanarShadowDemo
{
    class TestWindowApp : public Demo3DBase
    {
    public:
        TestWindowApp(const Vec2 resolution) : Demo3DBase(resolution) {}

        virtual void onInit()
        {
            Demo3DBase::onInit();
        }

        virtual void onRender(const unsigned int deltaTime)
        {
            Demo3DBase::onRender(deltaTime);

            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            if (!stopTime)
            {
                meshes[2]->rotate(Vec3(0.0f, 0.01f, 0.0f));
            }

            renderPlane();
            renderShadows();
            renderRegular();
        }
    private:

        virtual void initMaterials()
        {
            IResourceManager& rm = renderer->getResourceManager();

            materials.push_back(rm.createMaterial("Shaders/Shadows/pointLightProject.vs", "Shaders/Shadows/planarShadowProject.fs"));
            materials.push_back(rm.createMaterial("Shaders/Shadows/dirLightProject.vs", "Shaders/Shadows/planarShadowProject.fs"));
            materials.push_back(rm.createMaterial("Shaders/Shadows/spotLightProject.vs", "Shaders/Shadows/planarShadowProject.fs"));
            materials.push_back(rm.createMaterial("Shaders/basicDiffuse.vs", "Shaders/basicDiffuse.fs"));
            materials.push_back(rm.cloneMaterial(materials[3]));
            materials.push_back(rm.cloneMaterial(materials[3]));

            initMaterialProperty(*materials[3], "diffuse", Vec3(0.7f, 0.2f, 0.6f));
            initMaterialProperty(*materials[4], "diffuse", Vec3(0.2f, 0.6f, 0.6f));
            initMaterialProperty(*materials[5], "diffuse", Vec3(1.0f, 1.0f, 0.0f));

            Vec3PropertySharedPtr v3p;
            materials[0]->getProperty("pointLightPos", pointLightPosition);
            materials[0]->getProperty("planeNormal", v3p);
            planeNormals.push_back(v3p);
            materials[0]->getProperty("planePoint", v3p);
            planePoints.push_back(v3p);

            materials[1]->getProperty("dirLightDirection", dirLightDirection);
            materials[1]->getProperty("planeNormal", v3p);
            planeNormals.push_back(v3p);
            materials[1]->getProperty("planePoint", v3p);
            planePoints.push_back(v3p);

            materials[2]->getProperty("spotLightPos", spotLightPosition);
            materials[2]->getProperty("spotLightDirection", spotLightPosition);
            materials[2]->getProperty("spotLightAngle", spotLightAngle);
            materials[2]->getProperty("planeNormal", v3p);
            planeNormals.push_back(v3p);
            materials[2]->getProperty("planePoint", v3p);
            planePoints.push_back(v3p);
        }

        virtual void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            meshes.push_back(geometryFactory.createBlockMesh());
            meshes.push_back(geometryFactory.createRectangle());
            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/Sphere.3ds"));
            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/female_elf-3ds.3DS"));

            meshes[0]->scale(Vec3(2.0f));

            meshes[1]->scale(Vec3(10.0f));
            meshes[1]->rotate(Vec3(-3.14f * 0.5f, 0.0f, 0.0f));
            meshes[1]->translate(Vec3(5.0f, -5.0f, 0.0f));
            meshes[1]->setMaterial(materials[4]);

            meshes[2]->setMaterial(materials[5]);
            meshes[2]->scale(Vec3(0.05f));
            meshes[2]->translate(Vec3(5.0f));

            meshes[3]->scale(Vec3(0.03f));
            meshes[3]->rotate(Vec3(-3.14f / 2.0f, 0.0f, 0.0f));
            meshes[3]->translate(Vec3(5.0f, 0.0f, 0.0f));
        }

        void renderPlane()
        {
            scene.clear();

            scene.add(meshes[1]);

            meshes[1]->setMaterial(materials[4]);

            renderer->setStencilTest(true);
            renderer->setStencilOperation(StencilOperation::Keep, StencilOperation::Keep, StencilOperation::Replace);
            renderer->setStencilFunction(StencilFunction::Always, 0x1, 0xFF);
            renderer->render(scene, camera);
        }

        void renderShadows()
        {
            scene.clear();
            scene.add(meshes[0]);
            scene.add(meshes[3]);

            renderer->setStencilTest(true);
            renderer->setDepthTest(false);
            renderer->setStencilFunction(StencilFunction::Equal, 0x1, 0xFF);
            renderer->setStencilOperation(StencilOperation::Keep, StencilOperation::Increment, StencilOperation::Increment);
            renderer->setAlphaBlending(true, BlendMode::Normal);

            materials[0]->setProperty(pointLightPosition, meshes[2]->getPosition());
            materials[0]->setProperty(planeNormals[0],    Vec3(0.0f, 1.0f, 0.0f));
            materials[0]->setProperty(planePoints[0],     meshes[1]->getPosition());
            meshes[0]->setMaterial(materials[0]);
            meshes[3]->setMaterial(materials[0]);
            
            renderer->render(scene, camera);

            materials[1]->setProperty(dirLightDirection, Vec3(0.0f, -1.0f, 0.0f));
            materials[1]->setProperty(planeNormals[1], Vec3(0.0f, 1.0f, 0.0f));
            materials[1]->setProperty(planePoints[1], meshes[1]->getPosition());
            meshes[0]->setMaterial(materials[1]);
            meshes[3]->setMaterial(materials[1]);

            renderer->render(scene, camera);

            //materials[2]->setProperty(spotLightPosition, meshes[2]->getPosition());
            //materials[2]->setProperty(spotLightDirection, (-meshes[2]->getPosition()).normalize());
            //materials[2]->setProperty(spotLightAngle, 3.14f * 0.25);
            //materials[2]->setProperty(planeNormals[2], Vec3(0.0f, 1.0f, 0.0f));
            //materials[2]->setProperty(planePoints[2], meshes[1]->getPosition());
            //meshes[0]->setMaterial(materials[2]);
            //meshes[3]->setMaterial(materials[2]);

            //renderer->render(scene, camera);

            renderer->setAlphaBlending(false);
            renderer->setDepthTest(true);
        }


        void renderRegular()
        {
            scene.clear();
            scene.add(meshes[0]);
            scene.add(meshes[2]);
            scene.add(meshes[3]);

            meshes[0]->setMaterial(materials[3]);
            meshes[3]->setMaterial(materials[3]);

            renderer->setStencilTest(false);
            renderer->render(scene, camera);
        }

        vector<MeshNodeSharedPtr>     meshes;
        vector<IMaterial*>            materials;
        vector<Vec3PropertySharedPtr> cameraPositions;
        vector<Vec3PropertySharedPtr> planeNormals;
        vector<Vec3PropertySharedPtr> planePoints;
        Vec3PropertySharedPtr         pointLightPosition;
        Vec3PropertySharedPtr         spotLightPosition;
        Vec3PropertySharedPtr         spotLightDirection;
        FloatPropertySharedPtr        spotLightAngle;
        Vec3PropertySharedPtr         dirLightDirection;
    };

    void main()
    {
        WindowParameters params;
        params.width = 1600;
        params.height = 900;
        params.posX = 0;
        params.posY = 0;
        params.name = "PlanarShadow";
        TestWindowApp app(Vec2(static_cast<float>(params.width),
            static_cast<float>(params.height)));
        Window window(params, app);
        window.startRenderLoop();
    }
}