#pragma once

#include <vector>
#include <map>
#include <functional>
#include "Math/Vector.hpp"
#include "Core/IScene.hpp"
#include "Core/ICamera.hpp"
#include "Core/ITexture.hpp"
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
private:
    std::map<unsigned int, unsigned int> textures;
    OpenGLResourceManager    resourceManager;
    GeometryFactory          geometryFactory;
    IMaterial*               depthMat;
    IMaterial*               rectMat;
    IMaterial*               postProcessMat;
    IMaterial*               deferredShadingMat;
    IMaterial*               deferredShadingRectMat[4];
    shared_ptr<Rectangle>    r;
    shared_ptr<Rectangle>    postProcessRect;
    shared_ptr<Rectangle>    deferredShadingRect[4];
    IRenderTarget*           fb;
    IRenderTarget*           postProcessFbo;
    IRenderTarget*           deferredShadingFbo;
    ITexture*                shadowMap;
    ITexture*                postProcessTex;
    ITexture*                deferredShadingTex[3];
    PerspectiveCamera        lightCamera;
    Vec2                     resolution;
    IntPropertySharedPtr dsColorMap;
    IntPropertySharedPtr dsNormalMap;
    IntPropertySharedPtr dsDepthMap;
    typedef std::function<void(IMesh& mesh,
        ICamera& camera,
        ICamera& lightCamera)> PropertySetter;
    std::map<int, std::vector<PropertySetter>> systemPropertySetters;
    virtual void materialCreated(IMaterial& material);
    virtual void materialDestroyed(IMaterial& material);
    void initDeferredShading();
    void initPostProcessing();
    void initShadowMapping();
    unsigned int depthRenderBuffer;
    void render(std::vector<IMesh*>& meshes, ICamera& camera);
    void renderToTarget(std::vector<IMesh*>& meshes, ICamera& camera, IRenderTarget& renderTarget);
    void render(IMesh* mesh, ICamera& camera);
    void updateUniforms(const IMaterial& material);
    void renderToTexture(std::vector<IMesh*>& meshes, ICamera& camera, Vec4& viewport = Vec4(0, 0, 0, 0));
    void renderWithPostProcess(std::vector<IMesh*>& meshes, ICamera& camera);
    void renderDeferred(std::vector<IMesh*>& meshes, ICamera& camera);
    void postProcess(std::vector<IMesh*>& meshes, ICamera& camera);
};