#pragma once

#include <vector>
#include <map>
#include <functional>
#include "Math/Vector.hpp"
#include "Core/IScene.hpp"
#include "Core/ICamera.hpp"
#include "Core/ITexture.hpp"
#include "Core/MeshNode.hpp"
#include "Core/ContainerNode.hpp"
#include "Rendering/Variants/OpenGL/OpenGLMaterial.hpp"
#include "Geometry/Rectangle.hpp"
#include "Core/IResourceManager.hpp"
#include "Rendering/Variants/OpenGL/OpenGLResourceManager.hpp"
#include "Core/PerspectiveCamera.hpp"
#include "Geometry/GeometryFactory.hpp"

class OpenGLRenderer : public IResourceManagerNotify
{
public:
    OpenGLRenderer(const Vec2& resolution);
    ~OpenGLRenderer();
    void clear(const Vec4& color);
    void setDepthTest(const bool enabled);
    void setAlphaBlending(const bool enabled); //TODO: modes
    IResourceManager& getResourceManager();
    IGeometryFactory& getGeometryFactory();
    void render(IScene& scene, ICamera& camera);
    void renderToTarget(IScene& scene, ICamera& camera, IRenderTarget& renderTarget);
    void renderToTarget(IScene & scene, ICamera & camera, IRenderTarget & renderTarget, bool clear);
private:
    std::map<unsigned int, unsigned int> textures;
    OpenGLResourceManager      resourceManager;
    GeometryFactory            geometryFactory;
    IMaterial*                 depthMat;
    IMaterial*                 rectMat;
    IMaterial*                 postProcessMat;
    IMaterial*                 deferredShadingMat;
    IMaterial*                 deferredShadingRectMat[4];
    std::shared_ptr<MeshNode>  r;
    std::shared_ptr<MeshNode>  postProcessRect;
    std::shared_ptr<MeshNode>  deferredShadingRect[4];
    IRenderTarget*             fb;
    IRenderTarget*             postProcessFbo;
    IRenderTarget*             deferredShadingFbo;
    ITexture*                  shadowMap;
    ITexture*                  postProcessTex;
    ITexture*                  deferredShadingTex[3];
    PerspectiveCamera          lightCamera;
    Vec2                       resolution;
    IntPropertySharedPtr dsColorMap;
    IntPropertySharedPtr dsNormalMap;
    IntPropertySharedPtr dsDepthMap;
    unsigned int         depthRenderBuffer;

    typedef std::function<void(MeshNode& mesh,
        ICamera& camera,
        ICamera& lightCamera)> PropertySetter;
    std::map<int, std::vector<PropertySetter>> systemPropertySetters;

    virtual void materialCreated(IMaterial& material);
    virtual void materialDestroyed(IMaterial& material);
    void initDeferredShading();
    void initPostProcessing();
    void initShadowMapping();

    void updateUniforms(const IMaterial& material);

    // Single node render functions
    void render(std::shared_ptr<MeshNode> node, ICamera& camera);
    void render(std::shared_ptr<ContainerNode> node, ICamera& camera);
    void render(std::shared_ptr<IMesh> mesh, ICamera& camera);
    void renderToTarget(std::shared_ptr<IMesh> mesh, ICamera& camera, IRenderTarget& renderTarget);
    void renderToTexture(std::shared_ptr<MeshNode> node, ICamera& camera, Vec4& viewport = Vec4(0, 0, 0, 0));
    void renderDeferred(std::shared_ptr<MeshNode> node, ICamera& camera);

    typedef std::vector<std::shared_ptr<INode>> NodeList;
    // Node list render functions
    void render(NodeList& nodes, ICamera& camera);
    void renderToTarget(NodeList& nodes, ICamera& camera, IRenderTarget& renderTarget);
    
    void renderWithPostProcess(NodeList& nodes, ICamera& camera);
    void renderDeferred(NodeList& nodes, ICamera& camera);
    void postProcess(NodeList& nodes, ICamera& camera);
};