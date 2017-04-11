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

            dirLightDir    = Vec3(0.0f, -1.0f, 0.0f);
            spotLightPos   = Vec3(-2.0, 5.0f, -2.0f);
            spotLightDir   = -spotLightPos.normalize();
            spotLightAngle = 3.14f * 0.15f;
        }

        virtual void onRender(const unsigned int deltaTime)
        {
            Demo3DBase::onRender(deltaTime);

            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            if (!stopTime)
            {
                meshes[2]->rotate(Vec3(0.0f, 0.01f, 0.0f));
                pointLightPos = meshes[2]->getPosition();
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
            materials.push_back(rm.createMaterial("Shaders/basic.vs", "Shaders/basicLightSources.fs"));
            materials.push_back(rm.cloneMaterial(materials[3]));
            materials.push_back(rm.createMaterial("Shaders/basicDiffuse.vs", "Shaders/basicDiffuse.fs"));

            initMaterialProperty(*materials[3], "diffuse", Vec3(0.7f, 0.2f, 0.6f));
            initMaterialProperty(*materials[4], "diffuse", Vec3(0.2f, 0.6f, 0.6f));
            initMaterialProperty(*materials[5], "diffuse", Vec3(1.0f, 1.0f, 0.0f));

            pointLightPositions.push_back(getMaterialProperty<Vec3Property>(*materials[0], "pointLightPos"));
            planeNormals.push_back(getMaterialProperty<Vec3Property>(*materials[0], "planeNormal"));
            planePoints.push_back(getMaterialProperty<Vec3Property>(*materials[0], "planePoint"));

            dirLightDirections.push_back(getMaterialProperty<Vec3Property>(*materials[1], "dirLightDirection"));
            planeNormals.push_back(getMaterialProperty<Vec3Property>(*materials[1], "planeNormal"));
            planePoints.push_back(getMaterialProperty<Vec3Property>(*materials[1], "planePoint"));

            spotLightPositions.push_back(getMaterialProperty<Vec3Property>(*materials[2], "spotLightPos"));
            spotLightDirections.push_back(getMaterialProperty<Vec3Property>(*materials[2], "spotLightDir"));
            spotLightAngles.push_back(getMaterialProperty<FloatProperty>(*materials[2], "spotLightAngle"));
            planeNormals.push_back(getMaterialProperty<Vec3Property>(*materials[2], "planeNormal"));
            planePoints.push_back(getMaterialProperty<Vec3Property>(*materials[2], "planePoint"));

            for (int i = 3; i < 5; i++)
            {
                pointLightPositions.push_back(getMaterialProperty<Vec3Property>(*materials[i], "pointLightPos"));
                dirLightDirections.push_back(getMaterialProperty<Vec3Property>(*materials[i], "dirLightDirection"));
                spotLightPositions.push_back(getMaterialProperty<Vec3Property>(*materials[i], "spotLightPos"));
                spotLightDirections.push_back(getMaterialProperty<Vec3Property>(*materials[i], "spotLightDir"));
                spotLightAngles.push_back(getMaterialProperty<FloatProperty>(*materials[i], "spotLightAngle"));
            }
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

            materials[4]->setProperty(pointLightPositions[1], pointLightPos);
            materials[4]->setProperty(dirLightDirections[1], dirLightDir);
            materials[4]->setProperty(spotLightAngles[1], spotLightAngle);
            materials[4]->setProperty(spotLightDirections[1], spotLightDir);
            materials[4]->setProperty(spotLightPositions[1], spotLightPos);
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

            materials[0]->setProperty(pointLightPositions[0], pointLightPos);
            materials[0]->setProperty(planeNormals[0],    Vec3(0.0f, 1.0f, 0.0f));
            materials[0]->setProperty(planePoints[0],     meshes[1]->getPosition());
            meshes[0]->setMaterial(materials[0]);
            meshes[3]->setMaterial(materials[0]);
            
            renderer->render(scene, camera);

            materials[1]->setProperty(dirLightDirections[0], dirLightDir);
            materials[1]->setProperty(planeNormals[1], Vec3(0.0f, 1.0f, 0.0f));
            materials[1]->setProperty(planePoints[1], meshes[1]->getPosition());
            meshes[0]->setMaterial(materials[1]);
            meshes[3]->setMaterial(materials[1]);

            renderer->render(scene, camera);

            //materials[2]->setProperty(spotLightPositions[0], spotLightPos);
            //materials[2]->setProperty(spotLightDirections[0], spotLightDir);
            //materials[2]->setProperty(spotLightAngles[0], spotLightAngle);
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

            materials[3]->setProperty(pointLightPositions[2], pointLightPos);
            materials[3]->setProperty(dirLightDirections[2], dirLightDir);
            materials[3]->setProperty(spotLightAngles[2], spotLightAngle);
            materials[3]->setProperty(spotLightDirections[2], spotLightDir);
            materials[3]->setProperty(spotLightPositions[2], spotLightPos);

            meshes[0]->setMaterial(materials[3]);
            meshes[3]->setMaterial(materials[3]);

            renderer->setStencilTest(false);
            renderer->render(scene, camera);
        }

        vector<MeshNodeSharedPtr>      meshes;
        vector<IMaterial*>             materials;
        vector<Vec3PropertySharedPtr>  cameraPositions;
        vector<Vec3PropertySharedPtr>  planeNormals;
        vector<Vec3PropertySharedPtr>  planePoints;
        vector<Vec3PropertySharedPtr>  pointLightPositions;
        vector<Vec3PropertySharedPtr>  spotLightPositions;
        vector<Vec3PropertySharedPtr>  spotLightDirections;
        vector<FloatPropertySharedPtr> spotLightAngles;
        vector<Vec3PropertySharedPtr>  dirLightDirections;

        Vec3 dirLightDir;
        Vec3 pointLightPos;
        Vec3 spotLightPos;
        Vec3 spotLightDir;
        float spotLightAngle;
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