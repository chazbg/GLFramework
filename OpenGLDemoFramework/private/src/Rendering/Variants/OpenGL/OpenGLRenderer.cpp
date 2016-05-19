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

Renderer::Renderer(const Vec2& resolution) : lightCamera(3.0f / 4.0f, 16.0f / 9.0f, 1.0f, 1000.0f), resolution(resolution)
{
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(1);
    }

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

void Renderer::initDeferredShading()
{
    deferredShadingMat = resourceManager.createMaterial("Shaders/deferredShading.vs", "Shaders/deferredShading.fs");
    deferredShadingRectMat[0] = resourceManager.createMaterial("Shaders/tex.vs", "Shaders/tex.fs");
    deferredShadingRectMat[1] = resourceManager.cloneMaterial(deferredShadingRectMat[0]);
    deferredShadingRectMat[2] = resourceManager.cloneMaterial(deferredShadingRectMat[0]);
    deferredShadingRectMat[3] = resourceManager.createMaterial("Shaders/tex.vs", "Shaders/deferredShadingCompose.fs");
    deferredShadingRectMat[3]->setProperty("colorMap", 0);
    deferredShadingRectMat[3]->setProperty("normalMap", 1);
    deferredShadingRectMat[3]->setProperty("depthMap", 2);

    deferredShadingTex[0] = resourceManager.createTexture((unsigned int) resolution.x, (unsigned int) resolution.y, 3, false);
    deferredShadingTex[1] = resourceManager.createTexture((unsigned int) resolution.x, (unsigned int) resolution.y, 3, false);
    deferredShadingTex[2] = resourceManager.createTexture((unsigned int) resolution.x, (unsigned int) resolution.y, 3, true);

    deferredShadingRectMat[0]->addTexture(deferredShadingTex[0]);
    deferredShadingRectMat[1]->addTexture(deferredShadingTex[1]);
    deferredShadingRectMat[2]->addTexture(deferredShadingTex[2]);
    deferredShadingRectMat[3]->addTexture(deferredShadingTex[0]);
    deferredShadingRectMat[3]->addTexture(deferredShadingTex[1]);
    deferredShadingRectMat[3]->addTexture(deferredShadingTex[2]);

    deferredShadingRect[0] = new Rectangle(resourceManager);
    deferredShadingRect[0]->setMaterial(deferredShadingRectMat[0]);

    deferredShadingRect[1] = new Rectangle(resourceManager);
    deferredShadingRect[1]->setMaterial(deferredShadingRectMat[1]);

    deferredShadingRect[2] = new Rectangle(resourceManager);
    deferredShadingRect[2]->setMaterial(deferredShadingRectMat[2]);

    deferredShadingRect[3] = new Rectangle(resourceManager);
    deferredShadingRect[3]->setMaterial(deferredShadingRectMat[3]);

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

    postProcessRect = new Rectangle(resourceManager);
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

    r = new Rectangle(resourceManager, Vec2(0.5, 1), Vec2(1, 0.5));
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
    std::vector<IMesh*> children = mesh->getChildren();

    if (children.size() == 0)
    {
        mesh->getMaterial().setProperty("depthMvp", lightCamera.getViewProjectionMatrix() * mesh->getModelMatrix());
        mesh->getMaterial().setProperty("mvp", camera.getViewProjectionMatrix() * mesh->getModelMatrix());
        mesh->getMaterial().setProperty("modelToWorld", mesh->getModelMatrix());
        mesh->getMaterial().setProperty("modelView", camera.getViewMatrix() * mesh->getModelMatrix());

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
            render(children[i], camera);
        }
    }
}

void Renderer::postProcess(std::vector<IMesh*>& meshes, ICamera& camera)
{
    render(postProcessRect, camera);
}

void Renderer::updateUniforms(const IMaterial& material)
{
    unsigned int programId = reinterpret_cast<const OpenGLMaterial&>(material).getId();
    map<string, float> fUniforms = material.getFloatProperties();
    map<string, float>::iterator fIt;
    map<string, int> iUniforms = material.getIntProperties();
    map<string, int>::iterator iIt;
    map<string, unsigned int> uiUniforms = material.getUintProperties();
    map<string, unsigned int>::iterator uiIt;
    map<string, Vec3> vUniforms = material.getVec3Properties();
    map<string, Vec3>::iterator vIt;
    map<string, Matrix4> mUniforms = material.getMatrix4Properties();
    map<string, Matrix4>::iterator mIt;

    glUseProgram(programId);

    for (fIt = fUniforms.begin(); fIt != fUniforms.end(); fIt++)
    {
        int loc = glGetUniformLocation(programId, fIt->first.c_str());
        glUniform1f(loc, fIt->second);
    }

    for (iIt = iUniforms.begin(); iIt != iUniforms.end(); iIt++)
    {
        int loc = glGetUniformLocation(programId, iIt->first.c_str());
        glUniform1i(loc, iIt->second);
    }

    for (uiIt = uiUniforms.begin(); uiIt != uiUniforms.end(); uiIt++)
    {
        int loc = glGetUniformLocation(programId, uiIt->first.c_str());
        glUniform1ui(loc, uiIt->second);
    }

    for (vIt = vUniforms.begin(); vIt != vUniforms.end(); vIt++)
    {
        int loc = glGetUniformLocation(programId, vIt->first.c_str());
        glUniform3f(loc, vIt->second.x, vIt->second.y, vIt->second.z);
    }

    for (mIt = mUniforms.begin(); mIt != mUniforms.end(); mIt++)
    {
        int loc = glGetUniformLocation(programId, mIt->first.c_str());
        glUniformMatrix4fv(loc, 1, false, mIt->second.raw());
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

    //render(r, camera);
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
    render(deferredShadingRect[0], camera);

    glViewport(0, (GLsizei) quarterRes.y, (GLsizei) quarterRes.x, (GLsizei) quarterRes.y);
    render(deferredShadingRect[1], camera);

    glViewport((GLsizei) quarterRes.x, (GLsizei) quarterRes.y, (GLsizei) quarterRes.x, (GLsizei) quarterRes.y);
    render(deferredShadingRect[2], camera);

    glViewport(0, 0, (GLsizei) quarterRes.x, (GLsizei) quarterRes.y);
    render(deferredShadingRect[3], camera);

    glViewport(0, 0, (GLsizei) resolution.x, (GLsizei) resolution.y);
}