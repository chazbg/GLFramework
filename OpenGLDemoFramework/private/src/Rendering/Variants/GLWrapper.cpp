#include "Rendering/Renderer.hpp"
#include <GL/glew.h>
#include "Core/Shader.hpp"
#include <cstdio>
#include <cstdlib>

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

int Renderer::getTexId(const Texture* tex)
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

	return textures[tex->getId];
}