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
	glUseProgram(mesh->getMaterial().getId());

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
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei) mesh->getVertexCount());

	for (unsigned int i = 0; i < vbos.size(); i++)
	{
		if (vbos[i]->getId() != -1)
		{
			glDisableVertexAttribArray(i);
		}
	}
}