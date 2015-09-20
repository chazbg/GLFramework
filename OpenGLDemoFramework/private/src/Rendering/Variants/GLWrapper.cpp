#include "Rendering/Renderer.hpp"
#include <GL/glew.h>
#include "Core/Shader.hpp"
#include <cstdio>
#include <cstdlib>

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
    mesh->getMaterial().setProperty("mvp", camera.getViewMatrix() * mesh->getModelMatrix());
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