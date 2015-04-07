#include "GLWrapper.hpp"
#include <GL/glew.h>
#include "Shader.hpp"
#include <cstdio>
#include <cstdlib>

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
}

void RenderTexture(unsigned char* buffer, unsigned int width, unsigned int height)
{
	GLfloat texVertices[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, 0.5f
	};

	GLfloat texCoords[] = {
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};

	char vsh[] = "attribute vec4 position;							\
				attribute vec4 inputTextureCoordinate;				\
																	\
				varying vec2 textureCoordinate;						\
																	\
				void main()											\
				{													\
					gl_Position = position;							\
					textureCoordinate = inputTextureCoordinate.xy;	\
				}";

	char fsh[] = "varying highp vec2 textureCoordinate;				\
																	\
				uniform sampler2D videoFrame;						\
																	\
				void main()											\
				{													\
					gl_FragColor = texture2D(videoFrame, textureCoordinate);	\
				}";

}