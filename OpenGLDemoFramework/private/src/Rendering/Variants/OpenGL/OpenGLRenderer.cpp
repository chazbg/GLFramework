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
        systemPropertySetters[glMaterial.getId()].push_back([p](IMesh& mesh, ICamera& camera, ICamera& lightCamera)
        { 
            mesh.getMaterial().setProperty(p,
                lightCamera.getViewProjectionMatrix() * mesh.getModelMatrix());
        });
    }
    
    glMaterial.getProperty("mvp", p);
    if (p != 0)
    {
        systemPropertySetters[glMaterial.getId()].push_back([p](IMesh& mesh, ICamera& camera, ICamera& lightCamera)
        {
            mesh.getMaterial().setProperty(p, 
                camera.getViewProjectionMatrix() * mesh.getModelMatrix());
        });
    }

    glMaterial.getProperty("modelToWorld", p);
    if (p != 0)
    {
        systemPropertySetters[glMaterial.getId()].push_back([p](IMesh& mesh, ICamera& camera, ICamera& lightCamera)
        {
            mesh.getMaterial().setProperty(p,
                mesh.getModelMatrix());
        });
    }

    glMaterial.getProperty("modelView", p);
    if (p != 0)
    {
        systemPropertySetters[glMaterial.getId()].push_back([p](IMesh& mesh, ICamera& camera, ICamera& lightCamera)
        {
            mesh.getMaterial().setProperty(p,
                camera.getViewMatrix() * mesh.getModelMatrix());
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

    r->Scale(0.5f, 0.5f, 0.0f);
    r->Translate(0.5f, 0.5f, 0.0f);
    r->setMaterial(rectMat);

    fb = resourceManager.createRenderTarget();
    fb->addDepthTexture(shadowMap);

    lightCamera.setPosition(Vec3(0, 15, 25));
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

void OpenGLRenderer::setAlphaBlending(const bool enabled)
{
    if (enabled)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
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

void OpenGLRenderer::render(std::vector<IMesh*>& meshes, ICamera& camera)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        render(meshes[i], camera);
    }
}

void OpenGLRenderer::renderToTarget(std::vector<IMesh*>& meshes, ICamera & camera, IRenderTarget & renderTarget)
{
    unsigned int fbo = reinterpret_cast<OpenGLRenderTarget&>(renderTarget).getId();
    GLint originalFbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render(meshes, camera);

    glBindFramebuffer(GL_FRAMEBUFFER, originalFbo);
}

void OpenGLRenderer::render(IMesh* mesh, ICamera& camera)
{
    std::vector<shared_ptr<IMesh>>& children = mesh->getChildren();

    if (children.size() == 0)
    {
        OpenGLMaterial& glMaterial = reinterpret_cast<OpenGLMaterial&>(mesh->getMaterial());
        auto& propertySetters = systemPropertySetters[glMaterial.getId()];
        for (unsigned int i = 0; i < propertySetters.size(); ++i)
        {
            propertySetters[i](*mesh, camera, lightCamera);
        }

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
    else
    {
        for (unsigned int i = 0; i < children.size(); i++)
        {
            render(children[i].get(), camera);
        }
    }
}

void OpenGLRenderer::postProcess(std::vector<IMesh*>& meshes, ICamera& camera)
{
    render(postProcessRect.get(), camera);
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

void OpenGLRenderer::renderToTexture(std::vector<IMesh*>& meshes, ICamera& camera, Vec4& viewport)
{
    unsigned int fbo = reinterpret_cast<OpenGLRenderTarget*>(fb)->getId();

    std::vector<IMaterial*> originalMaterials;
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        originalMaterials.push_back(&meshes[i]->getMaterial());
        meshes[i]->setMaterial(depthMat);
    }

    renderToTarget(meshes, lightCamera, *fb);

    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        vector<const ITexture*> textures = originalMaterials[i]->getTextures();
        vector<const ITexture*>::iterator it = find(textures.begin(), textures.end(), shadowMap);
        if (textures.end() == it)
        {
            originalMaterials[i]->addTexture(shadowMap);
        }
        meshes[i]->setMaterial(originalMaterials[i]);
    }

    //render(&r, camera);
}

void OpenGLRenderer::renderWithPostProcess(std::vector<IMesh*>& meshes, ICamera& camera)
{
    renderToTarget(meshes, camera, *postProcessFbo);
    postProcess(meshes, camera);
}

void OpenGLRenderer::renderDeferred(std::vector<IMesh*>& meshes, ICamera& camera)
{
    Vec2 quarterRes = resolution / 2.0f;
    glViewport(0, 0, (GLsizei) resolution.x, (GLsizei)resolution.y);

    std::vector<IMaterial*> originalMaterials;
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        originalMaterials.push_back(&meshes[i]->getMaterial());
        meshes[i]->setMaterial(deferredShadingMat);
    }

    renderToTarget(meshes, camera, *deferredShadingFbo);

    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i]->setMaterial(originalMaterials[i]);
    }

    glViewport((GLsizei) quarterRes.x, 0, (GLsizei) quarterRes.x, (GLsizei) quarterRes.y);
    render(deferredShadingRect[0].get(), camera);

    glViewport(0, (GLsizei) quarterRes.y, (GLsizei) quarterRes.x, (GLsizei) quarterRes.y);
    render(deferredShadingRect[1].get(), camera);

    glViewport((GLsizei) quarterRes.x, (GLsizei) quarterRes.y, (GLsizei) quarterRes.x, (GLsizei) quarterRes.y);
    render(deferredShadingRect[2].get(), camera);

    glViewport(0, 0, (GLsizei) quarterRes.x, (GLsizei) quarterRes.y);
    render(deferredShadingRect[3].get(), camera);

    glViewport(0, 0, (GLsizei) resolution.x, (GLsizei) resolution.y);
}