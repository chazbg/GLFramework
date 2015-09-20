#include "Geometry/PlaneMesh.hpp"
#include "Core/PlaneGenerator.hpp"
#include "Core/Shader.hpp"

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
Mesh(),
width(_width), 
height(_height)
{
	//vertexCount = width * height * 6;
	//vertexBuffer = generatePlaneVertices(width, height);
	//generateNormals();
	//wireframeVertexBuffer = generateWireframe(vertexBuffer, width * height * 18);

	//programID = LoadShaders("Shaders/plane.vs", "Shaders/plane.fs");
	//timeID = glGetUniformLocation(programID, "time");

	//time = 0;

	//glUniform1ui(timeID, time);

	//glGenBuffers(1, &vertexBufferID);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	////glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * 4 * 2, wireframeVertexBuffer, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * 4, vertexBuffer, GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(
	//	0,
	//	3,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	0,
	//	BUFFER_OFFSET(0)
	//	);

	//glGenBuffers(1, &normalsBufferID);
	//glBindBuffer(GL_ARRAY_BUFFER, normalsBufferID);

	//glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * 4, normalsBuffer, GL_STATIC_DRAW);

	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(
	//	1,
	//	3,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	0,
	//	BUFFER_OFFSET(0)
	//	);

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
}

PlaneMesh::~PlaneMesh()
{
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

//void PlaneMesh::Render()
//{
//	UseProgram();
//	SetTime(time + 1);
//	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(
//		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//		3,                  // size
//		GL_FLOAT,           // type
//		GL_FALSE,           // normalized?
//		0,                  // stride
//		(void*)0            // array buffer offset
//		);
//	
//	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferID);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(
//		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//		3,                  // size
//		GL_FLOAT,           // type
//		GL_FALSE,           // normalized?
//		0,                  // stride
//		(void*)0            // array buffer offset
//		);
//	//glDrawArrays(GL_LINES, 0, vertexCount * 2);
//	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
//
//	glDisableVertexAttribArray(0);
//	glDisableVertexAttribArray(1);
//}