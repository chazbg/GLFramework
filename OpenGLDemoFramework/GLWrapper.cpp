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
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

	texShader = LoadShaders("tex.vsh", "tex.fsh");
	glGenTextures(1, &texName);
}

void GLWrapper::RenderTexture(unsigned char* buffer, unsigned int width, unsigned int height)
{
	static const GLfloat g_vertex_buffer_data[] = {
			-0.5f, -0.5f,
			1.0f, 1.0f,
			0.5f, -0.5f,
			1.0f, 0.0f,
			-0.5f, 0.5f,
			0.0f, 1.0f,
			0.5f, 0.5f,
			0.0f, 0.0f
	};

	//GLfloat texCoords[] = {
	//	1.0f, 1.0f,
	//	1.0f, 0.0f,
	//	0.0f, 1.0f,
	//	0.0f, 0.0f
	//};
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glUseProgram(texShader);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		16,                  // stride
		(void*)0            // array buffer offset
		);

	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		16,                  // stride
		(void*)16           // array buffer offset
		);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 3 indices starting at 0 -> 1 triangle

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//GLfloat texVertices[] = {
	//	-0.5f, -0.5f,
	//	0.5f, -0.5f,
	//	-0.5f, 0.5f,
	//	0.5f, 0.5f
	//};

	//GLfloat texCoords[] = {
	//	1.0f, 1.0f,
	//	1.0f, 0.0f,
	//	0.0f, 1.0f,
	//	0.0f, 0.0f
	//};

	//glUseProgram(texShader);
	////
	////printf("texShader %d\n", texShader);
	////printf("texName %d\n", texName);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texName);
	//glUniform1i(0, 0);

	//glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, texVertices);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, texCoords);
	//glEnableVertexAttribArray(1);
	//
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}