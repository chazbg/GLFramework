#include "Rendering/Renderer.hpp"
#include <GL/glew.h>
#include "Core/Shader.hpp"
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include "Rendering/Variants/OpenGL/OpenGLTexture.hpp"
#include "Rendering/Variants/OpenGL/OpenGLTextureCubemap.hpp"
#include "Rendering/Variants/OpenGL/OpenGLVertexBuffer.hpp"
#include "Rendering/Variants/OpenGL/OpenGLIndexBuffer.hpp"

#define BUFFER_OFFSET(i) ((void*)(i))

Renderer::Renderer(const Vec2& resolution) : 
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

void Renderer::materialCreated(IMaterial& material)
{
    OpenGLMaterial& glMaterial = reinterpret_cast<OpenGLMaterial&>(material);
    shared_ptr<IMaterialProperty<Matrix4>> p; 
    glMaterial.getProperty("depthMvp", p);
    if (p != 0)
    {
        systemPropertySetters[glMaterial.getId()].push_back([p](IMesh& mesh, ICamera& camera, ICamera& lightCamera)
        { 
            OpenGLMaterialProperty<Matrix4>* glProperty = reinterpret_cast<OpenGLMaterialProperty<Matrix4>*>(p.get());
            glProperty->value = lightCamera.getViewProjectionMatrix() * mesh.getModelMatrix();
        });
    }
    
    glMaterial.getProperty("mvp", p);
    if (p != 0)
    {
        systemPropertySetters[glMaterial.getId()].push_back([p](IMesh& mesh, ICamera& camera, ICamera& lightCamera)
        {
            OpenGLMaterialProperty<Matrix4>* glProperty = reinterpret_cast<OpenGLMaterialProperty<Matrix4>*>(p.get());
            glProperty->value = camera.getViewProjectionMatrix() * mesh.getModelMatrix();
        });
    }

    glMaterial.getProperty("modelToWorld", p);
    if (p != 0)
    {
        systemPropertySetters[glMaterial.getId()].push_back([p](IMesh& mesh, ICamera& camera, ICamera& lightCamera)
        {
            OpenGLMaterialProperty<Matrix4>* glProperty = reinterpret_cast<OpenGLMaterialProperty<Matrix4>*>(p.get());
            glProperty->value = mesh.getModelMatrix();
        });
    }

    glMaterial.getProperty("modelView", p);
    if (p != 0)
    {
        systemPropertySetters[glMaterial.getId()].push_back([p](IMesh& mesh, ICamera& camera, ICamera& lightCamera)
        {
            OpenGLMaterialProperty<Matrix4>* glProperty = reinterpret_cast<OpenGLMaterialProperty<Matrix4>*>(p.get());
            glProperty->value = camera.getViewMatrix() * mesh.getModelMatrix();
        });
    }
}

void Renderer::materialDestroyed(IMaterial& material)
{

}

void Renderer::initDeferredShading()
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

    deferredShadingFbo = new FrameBuffer();

    unsigned int fbo = deferredShadingFbo->getFbo();
    unsigned int texId[3];
    texId[0] = reinterpret_cast<OpenGLTexture*>(deferredShadingTex[0])->getId();
    texId[1] = reinterpret_cast<OpenGLTexture*>(deferredShadingTex[1])->getId();
    texId[2] = reinterpret_cast<OpenGLTexture*>(deferredShadingTex[2])->getId();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texId[0], 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, texId[1], 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texId[2], 0);

    GLenum drawBuffs[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, drawBuffs);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::initPostProcessing()
{
    postProcessMat = resourceManager.createMaterial("Shaders/tex.vs", "Shaders/postProcess.fs");
    postProcessMat->addTexture(postProcessTex);

    postProcessTex = resourceManager.createTexture((unsigned int) resolution.x, (unsigned int) resolution.y, 4, false);

    postProcessRect->setMaterial(postProcessMat);

    postProcessFbo = new FrameBuffer();

    unsigned int fbo = postProcessFbo->getFbo();
    unsigned int texId = reinterpret_cast<OpenGLTexture*>(postProcessTex)->getId();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (unsigned int) resolution.x, (unsigned int) resolution.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texId, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::initShadowMapping()
{
    rectMat = resourceManager.createMaterial("Shaders/tex.vs", "Shaders/tex.fs");

    depthMat = resourceManager.createMaterial("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");

    shadowMap = resourceManager.createTexture((unsigned int)resolution.x, (unsigned int)resolution.y, 4, true);

    rectMat->addTexture(shadowMap);

    depthMat->addTexture(shadowMap);

    r->Scale(0.5f, 0.5f, 0.0f);
    r->Translate(0.5f, 0.5f, 0.0f);
    r->setMaterial(rectMat);

    fb = new FrameBuffer();

    lightCamera.setPosition(Vec3(0, 15, 25));
}

Renderer::~Renderer()
{

}

void Renderer::clear(const Vec4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setDepthTest(const bool enabled)
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

void Renderer::setAlphaBlending(const bool enabled)
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

IResourceManager& Renderer::getResourceManager()
{
    return resourceManager;
}

IGeometryFactory& Renderer::getGeometryFactory()
{
    return geometryFactory;
}

void Renderer::render(IScene& scene, ICamera& camera)
{  
    //renderToTexture(scene.getChildren(), camera);
    render(scene.getChildren(), camera);
    //renderDeferred(scene.getChildren(), camera);
}

void Renderer::render(std::vector<IMesh*>& meshes, ICamera& camera)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        render(meshes[i], camera);
    }
}

void Renderer::render(IMesh* mesh, ICamera& camera)
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

void Renderer::postProcess(std::vector<IMesh*>& meshes, ICamera& camera)
{
    render(postProcessRect.get(), camera);
}

void Renderer::updateUniforms(const IMaterial& material)
{
    const OpenGLMaterial& glMaterial = reinterpret_cast<const OpenGLMaterial&>(material);
    unsigned int programId = glMaterial.getId();
    auto& fUniforms  = glMaterial.getFloatProperties();
    auto& iUniforms  = glMaterial.getIntProperties();
    auto& uiUniforms = glMaterial.getUintProperties();
    auto& v2Uniforms = glMaterial.getVec2Properties();
    auto& v3Uniforms = glMaterial.getVec3Properties();
    auto& m4Uniforms = const_cast<std::vector<std::shared_ptr<OpenGLMaterialProperty<Matrix4>>>&>(glMaterial.getMatrix4Properties());

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

void Renderer::renderToTexture(std::vector<IMesh*>& meshes, ICamera& camera, Vec4& viewport)
{
    unsigned int fbo = fb->getFbo();
    unsigned int texId = reinterpret_cast<OpenGLTexture*>(shadowMap)->getId();

    std::vector<IMaterial*> originalMaterials;
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        originalMaterials.push_back(&meshes[i]->getMaterial());
        meshes[i]->setMaterial(depthMat);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texId, 0);

    glDrawBuffer(GL_NONE); // No color buffer is drawn to.

                           // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("INVALID\n");

    render(meshes, lightCamera);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

void Renderer::renderWithPostProcess(std::vector<IMesh*>& meshes, ICamera& camera)
{
    unsigned int fbo = postProcessFbo->getFbo();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                                   // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("INVALID %d\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));

    render(meshes, camera);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    postProcess(meshes, camera);
}

void Renderer::renderDeferred(std::vector<IMesh*>& meshes, ICamera& camera)
{
    Vec2 quarterRes = resolution / 2.0f;
    glViewport(0, 0, (GLsizei) resolution.x, (GLsizei)resolution.y);

    unsigned int fbo = deferredShadingFbo->getFbo();

    std::vector<IMaterial*> originalMaterials;
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        originalMaterials.push_back(&meshes[i]->getMaterial());
        meshes[i]->setMaterial(deferredShadingMat);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("INVALID %d\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));

    render(meshes, camera);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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