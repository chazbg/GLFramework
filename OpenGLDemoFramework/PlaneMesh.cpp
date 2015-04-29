#include "PlaneMesh.hpp"
#include "PlaneGenerator.hpp"
#include "Shader.hpp"

#define BUFFER_OFFSET(i) ((void*)(i))

static float* generateWireframe(const float* vbuf, int length)
{
	float* wireframe = new float[length * 2];
	int j = 0;
	for (int i = 0; i < length; i += 9)
	{
		wireframe[j] = vbuf[i];
		wireframe[j+1] = vbuf[i+1];
		wireframe[j+2] = vbuf[i+2];

		wireframe[j+3] = vbuf[i+3];
		wireframe[j+4] = vbuf[i+4];
		wireframe[j+5] = vbuf[i+5];

		wireframe[j + 6] = vbuf[i + 3];
		wireframe[j + 7] = vbuf[i + 4];
		wireframe[j + 8] = vbuf[i + 5];

		wireframe[j + 9] = vbuf[i + 6];
		wireframe[j + 10] = vbuf[i + 7];
		wireframe[j + 11] = vbuf[i + 8];

		wireframe[j + 12] = vbuf[i + 6];
		wireframe[j + 13] = vbuf[i + 7];
		wireframe[j + 14] = vbuf[i + 8];

		wireframe[j + 15] = vbuf[i];
		wireframe[j + 16] = vbuf[i + 1];
		wireframe[j + 17] = vbuf[i + 2];
		j += 18;
	}
	return wireframe;
}

PlaneMesh::PlaneMesh(int _width, int _height) : 
width(_width), 
height(_height), 
vertexBuffer(generatePlaneVertices(width, height)),
vertexBufferWireframe(generateWireframe(vertexBuffer, width * height * 18))
{
	programID = LoadShaders("Shaders/plane.vs", "Shaders/plane.fs");
	timeID = glGetUniformLocation(programID, "time");

	time = 0;

	glUniform1ui(timeID, time);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	GenerateNormals();
	glBufferData(GL_ARRAY_BUFFER, width * height * 18 * 4 * 2, vertexBufferWireframe, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, width * height * 18 * 4, vertexBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		BUFFER_OFFSET(0)
		);

	UseProgram();
}

PlaneMesh::~PlaneMesh()
{
	delete[] vertexBufferWireframe;
	delete[] vertexBuffer;
}

void PlaneMesh::SetTime(GLuint time)
{
	this->time = time;
	glUniform1ui(timeID, time);
}

GLuint PlaneMesh::GetTime()
{
	return time;
}

void PlaneMesh::UseProgram()
{
	glUseProgram(programID);
}

void PlaneMesh::Render()
{
	UseProgram();
	SetTime(time + 1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glDrawArrays(GL_LINES, 0, width * height * 18 * 2);
	//glDrawArrays(GL_TRIANGLES, 0, width * height * 18);

	glDisableVertexAttribArray(0);
}

void PlaneMesh::GenerateNormals()
{
}