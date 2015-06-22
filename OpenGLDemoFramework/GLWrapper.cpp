#include "GLWrapper.hpp"
#include <GL/glew.h>
#include "Shader.hpp"
#include <cstdio>
#include <cstdlib>

unsigned int GLWrapper::texName = 0;
unsigned int GLWrapper::texShader = 0;

GLWrapper::GLWrapper()
{
}


GLWrapper::~GLWrapper()
{
}

void GLWrapper::ClearWindow()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWrapper::InitRenderer()
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

	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

	//texShader = LoadShaders("tex.vsh", "tex.fsh");
	//glGenTextures(1, &texName);
}

GLint programId;
GLuint fb;
void StoreState()
{
	//glGetIntegerv(GL_CURRENT_PROGRAM, &programId);
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
}

void RestoreState()
{
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glUseProgram(programId);
}
void GLWrapper::RenderTexture(unsigned char* buffer, unsigned int width, unsigned int height)
{
	const GLfloat g_vertex_buffer_data[] = {
			0.0f, -0.5f,
			1.0f, 1.0f,
			0.5f, -0.5f,
			1.0f, 0.0f,
			0.0f, 0.5f,
			0.0f, 1.0f,
			0.5f, 0.5f,
			0.0f, 0.0f
	};

	//////////////////////////////////
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glUseProgram(texShader);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		16,                  // stride
		(void*)0            // array buffer offset
		);

	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 1, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_TRUE,           // normalized?
		16,                  // stride
		(void*)16           // array buffer offset
		);

	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i(0, 0);
	
	
	//printf("%d\n", glGetError());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}