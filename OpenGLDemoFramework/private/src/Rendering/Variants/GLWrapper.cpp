#include "Rendering/Renderer.hpp"
#include <GL/glew.h>
#include "Core/Shader.hpp"
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#define BUFFER_OFFSET(i) ((void*)(i))

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
    depthMat = new ShaderMaterial("Shaders/depthMapping.vs", "Shaders/depthMapping.fs");
   
    TextureFactory texFactory;
    shadowMap = texFactory.createTexture(800, 800, 4, 0);

    rectMat->addTexture(shadowMap);
    depthMat->addTexture(shadowMap);

    r = new Rectangle(Vec2(0.5, 1), Vec2(1, 0.5));
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

void Renderer::render(IScene& scene, ICamera& camera)
{
    renderToTexture(scene.getChildren(), camera);
	render(scene.getChildren(), camera);
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
    mesh->getMaterial().setProperty("mv", camera.getViewMatrix() * mesh->getModelMatrix());
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
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vbos[0]->getVertexCount());

	for (unsigned int i = 0; i < vbos.size(); i++)
	{
		if (vbos[i]->getId() != -1)
		{
			glDisableVertexAttribArray(i);
		}
	}
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
        int loc = glGetAttribLocation(programId, fIt->first.c_str());
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

void Renderer::renderToTexture(std::vector<IMesh*>& meshes, ICamera& camera)
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 800, 800, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
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

    render(r, camera);
}