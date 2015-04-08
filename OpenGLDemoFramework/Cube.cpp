#include "Cube.hpp"
#include "Shader.hpp"
#include <cmath>
#include "Vector.hpp"

struct MyVertex
{
	float x, y, z;        //Vertex
	float nx, ny, nz;     //Normal
};

#define BUFFER_OFFSET(i) ((void*)(i))

const GLfloat Cube::vertices[] = {
	//front side
	//1
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	//2
	0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	//back side
	//1
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	//2
	1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	//bottom side
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,

	//top side
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	//right side
	//1
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	//2
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,

	//left side
	//1
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	//2
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
};

Cube::Cube()
{
	programID = LoadShaders("Shaders/cube.vs", "Shaders/cube.fs");
	timeID = glGetUniformLocation(programID, "time");

	time = 0;

	glUniform1ui(timeID, time);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	buffer = new GLfloat[216];
	GenerateNormals();
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(MyVertex), buffer, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(MyVertex),
		BUFFER_OFFSET(0)
		);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(MyVertex),
		BUFFER_OFFSET(12)
		);
	
	UseProgram();
}

Cube::~Cube()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Cube::SetTime(GLuint time)
{
	this->time = time;
	glUniform1ui(timeID, time);
}

GLuint Cube::GetTime()
{
	return time;
}

void Cube::UseProgram()
{
	glUseProgram(programID);
}

void Cube::Render()
{
	//UseProgram();
	SetTime(time + 1);

	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(
	//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	//	3,                  // size
	//	GL_FLOAT,           // type
	//	GL_FALSE,           // normalized?
	//	0,                  // stride
	//	(void*)0            // array buffer offset
	//	);

	glDrawArrays(GL_TRIANGLES, 0, 36); // 12 + 12 + 12 indices starting at 0 -> 4 + 4 + 4 triangles

	//glDisableVertexAttribArray(0);
}

void Cube::GenerateNormals()
{
	for (int i = 0, j = 0; i < 216, j < 108; i += 6, j += 3)
	{
		buffer[i] = vertices[j];
		buffer[i + 1] = vertices[j + 1];
		buffer[i + 2] = vertices[j + 2];
	}

	for (int i = 0, j = 0; i < 216, j < 108; i += 18, j += 9)
	{
		Vec3 a(vertices[j], vertices[j + 1], vertices[j + 2]);
		Vec3 b(vertices[j + 3], vertices[j + 4], vertices[j + 5]);
		Vec3 c(vertices[j + 6], vertices[j + 7], vertices[j + 8]);
		Vec3 cb = c - b;
		Vec3 ab = a - b;
		Vec3 res = cb * ab;

		buffer[i + 3] = res.x;
		buffer[i + 4] = res.y;
		buffer[i + 5] = res.z;

		buffer[i + 9] = res.x;
		buffer[i + 10] = res.y;
		buffer[i + 11] = res.z;

		buffer[i + 15] = res.x;
		buffer[i + 16] = res.y;
		buffer[i + 17] = res.z;
	}
}