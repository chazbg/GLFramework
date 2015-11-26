#include "Rendering/Renderer.hpp"
#include <GL/glew.h>
#include "Core/Shader.hpp"
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#define BUFFER_OFFSET(i) ((void*)(i))

#define FB_WIDTH 512
#define FB_HEIGHT 512

Renderer::Renderer()
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

    rectMat = new ShaderMaterial("Shaders/tex.vs", "Shaders/tex.fs");
    postProcessMat = new ShaderMaterial("Shaders/tex.vs", "Shaders/postProcess.fs");
    depthMat = new ShaderMaterial("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");

    deferredShadingMat = new ShaderMaterial("Shaders/deferredShading.vs", "Shaders/deferredShading.fs");
    deferredShadingRectMat[0] = new ShaderMaterial("Shaders/tex.vs", "Shaders/tex.fs");
    deferredShadingRectMat[1] = new ShaderMaterial("Shaders/tex.vs", "Shaders/tex.fs");
    deferredShadingRectMat[2] = new ShaderMaterial("Shaders/tex.vs", "Shaders/tex.fs");
    deferredShadingRectMat[3] = new ShaderMaterial("Shaders/tex.vs", "Shaders/deferredShadingCompose.fs");
    deferredShadingRectMat[3]->setProperty("colorMap", 0);
    deferredShadingRectMat[3]->setProperty("normalMap", 1);
    deferredShadingRectMat[3]->setProperty("depthMap", 2);

    TextureFactory texFactory;
    postProcessTex = texFactory.createTexture(FB_WIDTH, FB_HEIGHT, 4, 0);
    shadowMap = texFactory.createTexture(FB_WIDTH, FB_HEIGHT, 4, 0);

    deferredShadingTex[0] = texFactory.createTexture(FB_WIDTH, FB_HEIGHT, 3, 0);
    deferredShadingTex[1] = texFactory.createTexture(FB_WIDTH, FB_HEIGHT, 3, 0);
    deferredShadingTex[2] = texFactory.createTexture(FB_WIDTH, FB_HEIGHT, 3, 0);

    rectMat->addTexture(shadowMap);
    postProcessMat->addTexture(postProcessTex);
    depthMat->addTexture(shadowMap);

    deferredShadingRectMat[0]->addTexture(deferredShadingTex[0]);
    deferredShadingRectMat[1]->addTexture(deferredShadingTex[1]);
    deferredShadingRectMat[2]->addTexture(deferredShadingTex[2]);
    deferredShadingRectMat[3]->addTexture(deferredShadingTex[0]);
    deferredShadingRectMat[3]->addTexture(deferredShadingTex[1]);
    deferredShadingRectMat[3]->addTexture(deferredShadingTex[2]);

    r = new Rectangle(Vec2(0.5, 1), Vec2(1, 0.5));
    r->setMaterial(rectMat);

    postProcessRect = new Rectangle();
    postProcessRect->setMaterial(postProcessMat);

    deferredShadingRect[0] = new Rectangle();
    deferredShadingRect[0]->setMaterial(deferredShadingRectMat[0]);

    deferredShadingRect[1] = new Rectangle();
    deferredShadingRect[1]->setMaterial(deferredShadingRectMat[1]);

    deferredShadingRect[2] = new Rectangle();
    deferredShadingRect[2]->setMaterial(deferredShadingRectMat[2]);

    deferredShadingRect[3] = new Rectangle();
    deferredShadingRect[3]->setMaterial(deferredShadingRectMat[3]);

    fb = new FrameBuffer();
    postProcessFbo = new FrameBuffer();
    deferredShadingFbo = new FrameBuffer();

    lightCamera.setPosition(Vec3(0, 15, 25));

    {
        unsigned int fbo = postProcessFbo->getFbo();
        unsigned int texId = getTexId(postProcessTex);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FB_WIDTH, FB_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenRenderbuffers(1, &depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, FB_WIDTH, FB_HEIGHT);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texId, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    {
        unsigned int fbo = deferredShadingFbo->getFbo();
        unsigned int texId[3];
        texId[0] = getTexId(deferredShadingTex[0]);
        texId[1] = getTexId(deferredShadingTex[1]);
        texId[2] = getTexId(deferredShadingTex[2]);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glBindTexture(GL_TEXTURE_2D, texId[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FB_WIDTH, FB_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texId[0], 0);
       
        glBindTexture(GL_TEXTURE_2D, texId[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FB_WIDTH, FB_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, texId[1], 0);
        
        glBindTexture(GL_TEXTURE_2D, texId[2]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, FB_WIDTH, FB_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texId[2], 0);

        GLenum drawBuffs[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, drawBuffs);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
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

void Renderer::render(IScene& scene, ICamera& camera)
{
    glViewport(0, 0, 512, 512);
    renderDeferred(scene.getChildren(), camera);
    
    glViewport(512, 0, 512, 512);
    render(deferredShadingRect[0], camera);
    
    glViewport(0, 512, 512, 512);
    render(deferredShadingRect[1], camera);

    glViewport(512, 512, 512, 512);
    render(deferredShadingRect[2], camera);

    glViewport(0, 0, 512, 512);
    render(deferredShadingRect[3], camera);
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
    mesh->getMaterial().setProperty("depthMvp", lightCamera.getViewProjectionMatrix() * mesh->getModelMatrix());
    mesh->getMaterial().setProperty("mvp", camera.getViewProjectionMatrix() * mesh->getModelMatrix());
    mesh->getMaterial().setProperty("mv", mesh->getModelMatrix());
    updateUniforms(mesh->getMaterial());
    std::vector<const Texture*> textures = mesh->getMaterial().getTextures();

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        unsigned int texId = getTexId(textures[i]);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texId);
    }

    std::vector<IVertexBufferObject*> vbos = mesh->getVBOs();
    for (unsigned int i = 0; i < vbos.size(); i++)
    {
        if (vbos[i]->getId() != -1)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbos[i]->getId());
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, vbos[i]->getAttributeSize(), GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        }
    }

    //TODO: Strategy pattern
    IIndexBufferObject* ibo = mesh->getIBO();
    if (ibo)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->getId());
        glDrawElements(GL_TRIANGLES, (GLsizei)ibo->getIndexCount(), GL_UNSIGNED_INT, NULL);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vbos[0]->getVertexCount());
    }
    
    for (unsigned int i = 0; i < vbos.size(); i++)
    {
        if (vbos[i]->getId() != -1)
        {
            glDisableVertexAttribArray(i);
        }
    }
}

void Renderer::postProcess(std::vector<IMesh*>& meshes, ICamera& camera)
{
    render(postProcessRect, camera);
}

unsigned int Renderer::getTexId(const Texture* tex)
{
    if (textures.find(tex->getId()) == textures.end())
    {
        textures[tex->getId()] = 0;
        glGenTextures(1, &textures[tex->getId()]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[tex->getId()]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->getWidth(), tex->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->getData());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }

    return textures[tex->getId()];
}

void Renderer::updateUniforms(const IMaterial& material)
{
    unsigned int programId = material.getId();
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
    unsigned int texId = getTexId(shadowMap);

    std::vector<IMaterial*> originalMaterials;
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        originalMaterials.push_back(&meshes[i]->getMaterial());
        meshes[i]->setMaterial(depthMat);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glViewport(0, 0, 512, 512);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, FB_WIDTH, FB_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texId, 0);

    glDrawBuffer(GL_NONE); // No color buffer is drawn to.

                           // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("INVALID\n");

    render(meshes, lightCamera);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        vector<const Texture*> textures = originalMaterials[i]->getTextures();
        vector<const Texture*>::iterator it = find(textures.begin(), textures.end(), shadowMap);
        if (textures.end() == it)
        {
            originalMaterials[i]->addTexture(shadowMap);
        }
        meshes[i]->setMaterial(originalMaterials[i]);
    }

    glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
    render(r, camera);
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

}