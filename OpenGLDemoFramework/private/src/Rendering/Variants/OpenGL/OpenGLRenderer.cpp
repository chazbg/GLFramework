#include "Rendering/Variants/OpenGL/OpenGLRenderer.hpp"
#include "Rendering/Variants/OpenGL/OpenGLTexture.hpp"
#include "Rendering/Variants/OpenGL/OpenGLTextureCubemap.hpp"
#include "Rendering/Variants/OpenGL/OpenGLVertexBuffer.hpp"
#include "Rendering/Variants/OpenGL/OpenGLIndexBuffer.hpp"
#include "Rendering/Variants/OpenGL/OpenGLRenderTarget.hpp"
#include "Core/Shader.hpp"
#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#define BUFFER_OFFSET(i) ((void*)(i))

OpenGLRenderer::OpenGLRenderer(const Vec2& resolution) :
    resourceManager(*this), 
    geometryFactory(resourceManager),
    r(geometryFactory.createRectangle()),
    postProcessRect(geometryFactory.createRectangle()),
    lightCamera(3.0f / 4.0f, 16.0f / 9.0f, 1.0f, 1000.0f), 
    resolution(resolution)
{
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    initDeferredShading();
    initPostProcessing();
    initShadowMapping();
}

void OpenGLRenderer::materialCreated(IMaterial& material)
{
    OpenGLMaterial& glMaterial = reinterpret_cast<OpenGLMaterial&>(material);
    shared_ptr<IMaterialProperty<Matrix4>> p; 
    glMaterial.getProperty("depthMvp", p);
    if (p != 0)
    {
        systemPropertySetters[glMaterial.getId()].push_back([p](Matrix4& modelToWorld, IMesh& mesh, ICamera& camera, ICamera& lightCamera)
        { 
            mesh.getMaterial().setProperty(p,
                lightCamera.getViewProjectionMatrix() * modelToWorld);
        });
    }
    
    glMaterial.getProperty("mvp", p);
    if (p != 0)
    {
        systemPropertySetters[glMaterial.getId()].push_back([p](Matrix4& modelToWorld, IMesh& mesh, ICamera& camera, ICamera& lightCamera)
        {
            mesh.getMaterial().setProperty(p,
                camera.getViewProjectionMatrix() * modelToWorld);
        });
    }

    glMaterial.getProperty("modelToWorld", p);
    if (p != 0)
    {
        systemPropertySetters[glMaterial.getId()].push_back([p](Matrix4& modelToWorld, IMesh& mesh, ICamera& camera, ICamera& lightCamera)
        {
            mesh.getMaterial().setProperty(p,
                modelToWorld);
        });
    }

    glMaterial.getProperty("modelView", p);
    if (p != 0)
    {
        systemPropertySetters[glMaterial.getId()].push_back([p](Matrix4& modelToWorld, IMesh& mesh, ICamera& camera, ICamera& lightCamera)
        {
            mesh.getMaterial().setProperty(p,
                camera.getViewMatrix() * modelToWorld);
        });
    }
}

void OpenGLRenderer::materialDestroyed(IMaterial& material)
{

}

void OpenGLRenderer::initDeferredShading()
{
    deferredShadingMat = resourceManager.createMaterial("Shaders/deferredShading.vs", "Shaders/deferredShading.fs");
    deferredShadingRectMat[0] = resourceManager.createMaterial("Shaders/tex.vs", "Shaders/tex.fs");
    deferredShadingRectMat[1] = resourceManager.cloneMaterial(deferredShadingRectMat[0]);
    deferredShadingRectMat[2] = resourceManager.cloneMaterial(deferredShadingRectMat[0]);
    deferredShadingRectMat[3] = resourceManager.createMaterial("Shaders/tex.vs", "Shaders/deferredShadingCompose.fs");
    deferredShadingRectMat[3]->getProperty("colorMap", dsColorMap);
    deferredShadingRectMat[3]->getProperty("normalMap", dsNormalMap);
    deferredShadingRectMat[3]->getProperty("depthMap", dsDepthMap);
    deferredShadingRectMat[3]->setProperty(dsColorMap, 0);
    deferredShadingRectMat[3]->setProperty(dsNormalMap, 1);
    deferredShadingRectMat[3]->setProperty(dsNormalMap, 2);

    deferredShadingTex[0] = resourceManager.createTexture((unsigned int) resolution.x, (unsigned int) resolution.y, 3, false);
    deferredShadingTex[1] = resourceManager.createTexture((unsigned int) resolution.x, (unsigned int) resolution.y, 3, false);
    deferredShadingTex[2] = resourceManager.createTexture((unsigned int) resolution.x, (unsigned int) resolution.y, 3, true);

    deferredShadingRectMat[0]->addTexture(deferredShadingTex[0]);
    deferredShadingRectMat[1]->addTexture(deferredShadingTex[1]);
    deferredShadingRectMat[2]->addTexture(deferredShadingTex[2]);
    deferredShadingRectMat[3]->addTexture(deferredShadingTex[0]);
    deferredShadingRectMat[3]->addTexture(deferredShadingTex[1]);
    deferredShadingRectMat[3]->addTexture(deferredShadingTex[2]);

    for (int i = 0; i < 4; i++)
    {
        deferredShadingRect[i] = geometryFactory.createRectangle();
        deferredShadingRect[i]->setMaterial(deferredShadingRectMat[i]);
    }

    deferredShadingFbo = resourceManager.createRenderTarget();
    deferredShadingFbo->addColorTexture(deferredShadingTex[0]);
    deferredShadingFbo->addColorTexture(deferredShadingTex[1]);
    deferredShadingFbo->addDepthTexture(deferredShadingTex[2]);
}

void OpenGLRenderer::initPostProcessing()
{
    postProcessMat = resourceManager.createMaterial("Shaders/tex.vs", "Shaders/postProcess.fs");
    postProcessTex = resourceManager.createTexture(static_cast<unsigned int>(resolution.x), 
                                                   static_cast<unsigned int>(resolution.y), 4, false);
    postProcessMat->addTexture(postProcessTex);

    Vec2PropertySharedPtr resProperty;
    postProcessMat->getProperty("resolution", resProperty);
    postProcessMat->setProperty(resProperty, resolution);

    postProcessRect->setMaterial(postProcessMat);

    postProcessFbo = resourceManager.createRenderTarget();
    postProcessFbo->addColorTexture(postProcessTex);
    postProcessFbo->addDepthTexture(resourceManager.createTexture(
        static_cast<unsigned int>(resolution.x),
        static_cast<unsigned int>(resolution.y), 3, true));
}

void OpenGLRenderer::initShadowMapping()
{
    rectMat = resourceManager.createMaterial("Shaders/tex.vs", "Shaders/tex.fs");
    depthMat = resourceManager.createMaterial("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
    shadowMap = resourceManager.createTexture(
        static_cast<unsigned int>(resolution.x), 
        static_cast<unsigned int>(resolution.y), 4, true);

    rectMat->addTexture(shadowMap);
    depthMat->addTexture(shadowMap);

    r->scale(Vec3(0.5f, 0.5f, 0.0f));
    r->translate(Vec3(0.5f, 0.5f, 0.0f));
    r->setMaterial(rectMat);

    fb = resourceManager.createRenderTarget();
    fb->addDepthTexture(shadowMap);

    lightCamera.setPosition(Vec3(0, 15, 25));
}

int OpenGLRenderer::getStencilOp(StencilOperation op)
{
    const int stencilOperations[] = {
        GL_KEEP,
        GL_ZERO,
        GL_REPLACE,
        GL_INCR,
        GL_INCR_WRAP,
        GL_DECR,
        GL_DECR_WRAP,
        GL_INVERT
    };
    
    return stencilOperations[static_cast<int>(op)];
}

int OpenGLRenderer::getStencilFunc(StencilFunction f)
{
    const int stencilFunctions[] = {
        GL_NEVER,
        GL_LESS,
        GL_LEQUAL,
        GL_GREATER,
        GL_GEQUAL,
        GL_EQUAL,
        GL_NOTEQUAL,
        GL_ALWAYS
    };

    return stencilFunctions[static_cast<int>(f)];
}

OpenGLRenderer::~OpenGLRenderer()
{

}

void OpenGLRenderer::clear(const Vec4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::setDepthTest(const bool enabled)
{
    if (enabled)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

void OpenGLRenderer::setStencilTest(const bool enabled)
{
    if (enabled)
    {
        glEnable(GL_STENCIL_TEST);

    }
    else
    {
        glDisable(GL_STENCIL_TEST);
    }
}

void OpenGLRenderer::setStencilOperation(StencilOperation sfail, StencilOperation dpfail, StencilOperation dppass)
{
    glStencilOp(getStencilOp(sfail), getStencilOp(dpfail), getStencilOp(dppass));
}

void OpenGLRenderer::setStencilFunction(StencilFunction f, int referenceValue, int mask)
{
    glStencilFunc(getStencilFunc(f), referenceValue, mask);
}

void OpenGLRenderer::setAlphaBlending(const bool enabled, BlendMode mode)
{
    if (enabled)
    {
        glEnable(GL_BLEND);
        int sfactor = GL_SRC_ALPHA;
        int dfactor = GL_ONE_MINUS_SRC_ALPHA;

        //TODO: Replace with array lookup
        switch (mode)
        {
        case BlendMode::Additive:
        {
            sfactor = GL_SRC_ALPHA;
            dfactor = GL_ONE;
            break;
        }
        case BlendMode::Normal:
        {
            sfactor = GL_SRC_ALPHA;
            dfactor = GL_ONE_MINUS_SRC_ALPHA;
            break;
        }
        default:
        {
            sfactor = GL_SRC_ALPHA;
            dfactor = GL_ONE_MINUS_SRC_ALPHA;
            break;
        }
        }
        glBlendFunc(sfactor, dfactor);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

IResourceManager& OpenGLRenderer::getResourceManager()
{
    return resourceManager;
}

IGeometryFactory& OpenGLRenderer::getGeometryFactory()
{
    return geometryFactory;
}

void OpenGLRenderer::render(IScene& scene, ICamera& camera)
{  
    //renderToTexture(scene.getChildren(), camera);
    render(scene.getChildren(), camera);
    //renderWithPostProcess(scene.getChildren(), camera);
    //renderDeferred(scene.getChildren(), camera);
}

void OpenGLRenderer::renderToTarget(IScene & scene, ICamera & camera, IRenderTarget & renderTarget)
{
    unsigned int fbo = reinterpret_cast<OpenGLRenderTarget&>(renderTarget).getId();
    GLint originalFbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render(scene, camera);

    glBindFramebuffer(GL_FRAMEBUFFER, originalFbo);
}

void OpenGLRenderer::renderToTarget(IScene & scene, ICamera & camera, IRenderTarget & renderTarget, bool clear)
{
    unsigned int fbo = reinterpret_cast<OpenGLRenderTarget&>(renderTarget).getId();
    GLint originalFbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    if (clear)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    render(scene, camera);

    glBindFramebuffer(GL_FRAMEBUFFER, originalFbo);
}

void OpenGLRenderer::render(std::vector<std::shared_ptr<INode>>& nodes, ICamera & camera)
{
    for (auto node : nodes)
    {
        switch (node->getNodeType())
        {
        case NodeType::Container:
        {
            auto actualNode = std::static_pointer_cast<ContainerNode>(node);
            render(actualNode, camera);
            break;
        }
        case NodeType::Mesh:
        {
            auto actualNode = std::static_pointer_cast<MeshNode>(node);
            render(actualNode, camera);
            break;
        }
        default:
            break;
        }
    }
}

void OpenGLRenderer::renderToTarget(NodeList& nodes, ICamera & camera, IRenderTarget & renderTarget)
{
    unsigned int fbo = reinterpret_cast<OpenGLRenderTarget&>(renderTarget).getId();
    GLint originalFbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render(nodes, camera);

    glBindFramebuffer(GL_FRAMEBUFFER, originalFbo);
}

void OpenGLRenderer::render(std::shared_ptr<MeshNode> node, ICamera& camera)
{
    auto meshes = node->getMeshes();

    for (auto mesh : meshes)
    {
        OpenGLMaterial& glMaterial = reinterpret_cast<OpenGLMaterial&>(mesh->getMaterial());
        auto& propertySetters = systemPropertySetters[glMaterial.getId()];
        for (unsigned int i = 0; i < propertySetters.size(); ++i)
        {
            propertySetters[i](node->getModelToWorldMatrix(), *mesh, camera, lightCamera);
        }

        render(mesh, camera);
    }


    for (auto child : node->getChildren())
    {
        render(child, camera);
    }
}

void OpenGLRenderer::render(std::shared_ptr<ContainerNode> node, ICamera & camera)
{
    for (auto child : node->getChildren())
    {
        switch (child->getNodeType())
        {
        case NodeType::Mesh:
        {
            auto actualNode = std::static_pointer_cast<MeshNode>(child);
            render(actualNode, camera);
            break;
        }
        default:
            break;
        }
    }
}

void OpenGLRenderer::render(std::shared_ptr<IMesh> mesh, ICamera& camera)
{
    updateUniforms(mesh->getMaterial());
    std::vector<const ITexture*> textures = mesh->getMaterial().getTextures();
    std::vector<const ITextureCubemap*> textureCubemaps = mesh->getMaterial().getTextureCubemaps();

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        const OpenGLTexture* tex = reinterpret_cast<const OpenGLTexture*>(textures[i]);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, tex->getId());
    }

    for (unsigned int i = textures.size(); i < textures.size() + textureCubemaps.size(); i++)
    {
        const OpenGLTextureCubemap* tex = reinterpret_cast<const OpenGLTextureCubemap*>(textureCubemaps[i - textures.size()]);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex->getId());
    }

    std::vector<const IVertexBuffer*> vbos = mesh->getVBOs();
    for (unsigned int i = 0; i < vbos.size(); i++)
    {
        const OpenGLVertexBuffer* buf = reinterpret_cast<const OpenGLVertexBuffer*>(vbos[i]);
        if (buf)
        {
            glBindBuffer(GL_ARRAY_BUFFER, buf->getId());
            glEnableVertexAttribArray(buf->getLocation());
            glVertexAttribPointer(buf->getLocation(), buf->getAttributeSize(), GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        }
    }

    //TODO: Strategy pattern
    const IIndexBuffer* ibo = mesh->getIBO();
    if (ibo)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, reinterpret_cast<const OpenGLIndexBuffer*>(ibo)->getId());
        glDrawElements(GL_TRIANGLES, ibo->getIndexCount(), GL_UNSIGNED_INT, NULL);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, vbos[0]->getVertexCount());
    }

    for (unsigned int i = 0; i < vbos.size(); i++)
    {
        const OpenGLVertexBuffer* buf = reinterpret_cast<const OpenGLVertexBuffer*>(vbos[i]);
        if (buf)
        {
            glDisableVertexAttribArray(buf->getLocation());
        }
    }
}

void OpenGLRenderer::renderToTarget(std::shared_ptr<IMesh> mesh, ICamera & camera, IRenderTarget & renderTarget)
{
    unsigned int fbo = reinterpret_cast<OpenGLRenderTarget&>(renderTarget).getId();
    GLint originalFbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render(mesh, camera);

    glBindFramebuffer(GL_FRAMEBUFFER, originalFbo);
}

void OpenGLRenderer::renderToTexture(std::shared_ptr<MeshNode> node, ICamera & camera, Vec4 & viewport)
{
    for (auto mesh : node->getMeshes())
    {
        auto originalMaterial = &mesh->getMaterial();
        mesh->setMaterial(depthMat);

        renderToTarget(mesh, lightCamera, *fb);

        auto& textures = originalMaterial->getTextures();
        auto it = find(textures.begin(), textures.end(), shadowMap);

        if (textures.end() == it)
        {
            originalMaterial->addTexture(shadowMap);
        }

        mesh->setMaterial(originalMaterial);
    }

    for (auto child : node->getChildren())
    {
        renderToTexture(child, camera, viewport);
    }
}

void OpenGLRenderer::renderDeferred(std::shared_ptr<MeshNode> node, ICamera & camera)
{
    for (auto mesh : node->getMeshes())
    {
        auto originalMaterial = &mesh->getMaterial();
        mesh->setMaterial(deferredShadingMat);

        renderToTarget(mesh, camera, *deferredShadingFbo);

        mesh->setMaterial(originalMaterial);
    }

    for (auto child : node->getChildren())
    {
        renderDeferred(child, camera);
    }
}

void OpenGLRenderer::postProcess(NodeList& nodes, ICamera& camera)
{
    render(postProcessRect, camera);
}

void OpenGLRenderer::updateUniforms(const IMaterial& material)
{
    const OpenGLMaterial& glMaterial = reinterpret_cast<const OpenGLMaterial&>(material);
    unsigned int programId = glMaterial.getId();
    auto& fUniforms  = glMaterial.getFloatProperties();
    auto& iUniforms  = glMaterial.getIntProperties();
    auto& uiUniforms = glMaterial.getUintProperties();
    auto& v2Uniforms = glMaterial.getVec2Properties();
    auto& v3Uniforms = glMaterial.getVec3Properties();
    auto& m4Uniforms = const_cast<std::vector<OpenGLMat4PropertySharedPtr>&>(glMaterial.getMatrix4Properties());

    glUseProgram(programId);

    for (unsigned int i = 0; i < fUniforms.size(); ++i)
    {
        glUniform1f(fUniforms[i]->location, fUniforms[i]->value);
    }

    for (unsigned int i = 0; i < iUniforms.size(); ++i)
    {
        glUniform1i(iUniforms[i]->location, iUniforms[i]->value);
    }

    for (unsigned int i = 0; i < uiUniforms.size(); ++i)
    {
        glUniform1ui(uiUniforms[i]->location, uiUniforms[i]->value);
    }

    for (unsigned int i = 0; i < v2Uniforms.size(); ++i)
    {
        glUniform2f(v2Uniforms[i]->location, v2Uniforms[i]->value.x, v2Uniforms[i]->value.y);
    }

    for (unsigned int i = 0; i < v3Uniforms.size(); ++i)
    {
        glUniform3f(v3Uniforms[i]->location, v3Uniforms[i]->value.x,
                                             v3Uniforms[i]->value.y, 
                                             v3Uniforms[i]->value.z);
    }

    for (unsigned int i = 0; i < m4Uniforms.size(); ++i)
    {
        glUniformMatrix4fv(m4Uniforms[i]->location, 1, false, m4Uniforms[i]->value.raw());
    }
}

void OpenGLRenderer::renderWithPostProcess(NodeList& nodes, ICamera& camera)
{
    renderToTarget(nodes, camera, *postProcessFbo);
    postProcess(nodes, camera);
}

void OpenGLRenderer::renderDeferred(NodeList& nodes, ICamera& camera)
{
    for (auto node : nodes)
    {
        switch (node->getNodeType())
        {
        case NodeType::Mesh:
        {
            auto actualNode = std::static_pointer_cast<MeshNode>(node);
            renderDeferred(actualNode, camera);
            break;
        }
        default:
            break;
        }
    }

    Vec2 quarterRes = resolution / 2.0f;
    glViewport(0, 0, (GLsizei)resolution.x, (GLsizei)resolution.y);

    glViewport((GLsizei) quarterRes.x, 0, (GLsizei) quarterRes.x, (GLsizei) quarterRes.y);
    render(deferredShadingRect[0], camera);

    glViewport(0, (GLsizei) quarterRes.y, (GLsizei) quarterRes.x, (GLsizei) quarterRes.y);
    render(deferredShadingRect[1], camera);

    glViewport((GLsizei) quarterRes.x, (GLsizei) quarterRes.y, (GLsizei) quarterRes.x, (GLsizei) quarterRes.y);
    render(deferredShadingRect[2], camera);

    glViewport(0, 0, (GLsizei) quarterRes.x, (GLsizei) quarterRes.y);
    render(deferredShadingRect[3], camera);

    glViewport(0, 0, (GLsizei) resolution.x, (GLsizei) resolution.y);
}