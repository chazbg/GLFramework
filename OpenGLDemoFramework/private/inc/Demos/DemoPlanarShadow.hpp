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
            meshes[2]->rotate(Vec3(0.0f, 0.01f, 0.0f));

            renderPlane();
            renderShadows();
            renderRegular();
        }
    private:

        virtual void initMaterials()
        {
            IResourceManager& rm = renderer->getResourceManager();

            materials.push_back(rm.createMaterial("Shaders/Shadows/planarShadowProject.vs",
                "Shaders/Shadows/planarShadowProject.fs"));
            materials.push_back(rm.createMaterial("Shaders/basicDiffuse.vs",
                "Shaders/basicDiffuse.fs"));
            materials.push_back(rm.cloneMaterial(materials[1]));
            materials.push_back(rm.cloneMaterial(materials[1]));

            initMaterialProperty(*materials[1], "diffuse", Vec3(0.7f, 0.2f, 0.6f));
            initMaterialProperty(*materials[2], "diffuse", Vec3(0.2f, 0.6f, 0.6f));
            initMaterialProperty(*materials[3], "diffuse", Vec3(1.0f, 1.0f, 0.0f));

            Vec3PropertySharedPtr v3p;
            materials[0]->getProperty("cameraPos", v3p);
            cameraPositions.push_back(v3p);
            materials[0]->getProperty("pointLightPos", v3p);
            lightPositions.push_back(v3p);
            materials[0]->getProperty("planeNormal", v3p);
            planeNormals.push_back(v3p);
            materials[0]->getProperty("planePoint", v3p);
            planePoints.push_back(v3p);
        }

        virtual void initGeometry()
        {
            IGeometryFactory& geometryFactory = renderer->getGeometryFactory();
            meshes.push_back(geometryFactory.createBlockMesh());
            meshes.push_back(geometryFactory.createRectangle());
            meshes.push_back(geometryFactory.createCustomGeometry("3DAssets/Sphere.3ds"));

            meshes[0]->scale(Vec3(2.0f));

            meshes[1]->scale(Vec3(10.0f));
            meshes[1]->rotate(Vec3(-3.14f * 0.5f, 0.0f, 0.0f));
            meshes[1]->translate(Vec3(10.0f, -10.0f, 0.0f));
            meshes[1]->setMaterial(materials[2]);

            meshes[2]->setMaterial(materials[3]);
            meshes[2]->scale(Vec3(0.05f));
            meshes[2]->translate(Vec3(10.0f));
        }

        void renderPlane()
        {
            scene.clear();

            scene.add(meshes[1]);

            meshes[1]->setMaterial(materials[2]);

            renderer->setStencilTest(true);
            renderer->setStencilOperation(StencilOperation::Keep, StencilOperation::Keep, StencilOperation::Replace);
            renderer->setStencilFunction(StencilFunction::Always, 0x1, 0xFF);
            renderer->setStencilMask(0xFF);
            renderer->setDepthMask(false);
            renderer->render(scene, camera);
        }

        void renderShadows()
        {
            scene.clear();
            scene.add(meshes[0]);

            materials[0]->setProperty(lightPositions[0], meshes[2]->getPosition());
            materials[0]->setProperty(planeNormals[0],   Vec3(0.0f, 1.0f, 0.0f));
            materials[0]->setProperty(planePoints[0],    meshes[1]->getPosition());
            meshes[0]->setMaterial(materials[0]);
            
            renderer->setStencilTest(true);
            renderer->setStencilFunction(StencilFunction::Equal, 0x1, 0xFF);
            renderer->setStencilMask(0x0);
            renderer->setDepthMask(true);
            renderer->render(scene, camera);
            renderer->setStencilMask(0xFF);
        }


        void renderRegular()
        {
            scene.clear();
            scene.add(meshes[0]);
            scene.add(meshes[2]);

            meshes[0]->setMaterial(materials[1]);

            renderer->setStencilTest(false);
            renderer->render(scene, camera);
        }

        vector<MeshNodeSharedPtr>     meshes;
        vector<IMaterial*>            materials;
        vector<Vec3PropertySharedPtr> cameraPositions;
        vector<Vec3PropertySharedPtr> lightPositions;
        vector<Vec3PropertySharedPtr> planeNormals;
        vector<Vec3PropertySharedPtr> planePoints;
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